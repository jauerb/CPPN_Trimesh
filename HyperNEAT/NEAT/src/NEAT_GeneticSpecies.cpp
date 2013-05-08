#include "NEAT_GeneticSpecies.h"

#include "NEAT_Globals.h"
#include "NEAT_Random.h"

#include "NEAT_GeneticIndividual.h"


namespace NEAT
{

    GeneticSpecies::GeneticSpecies(shared_ptr<GeneticIndividual> firstIndividual)
            :
            age(0),
            ageOfLastImprovement(0),
            speciesFitness(0),
            oldAverageFitness(0)
    {
        ID = Globals::getSingleton()->generateSpeciesID();
        firstIndividual->setSpeciesID(ID);
        bestIndividualEver = firstIndividual;
    }

    GeneticSpecies::~GeneticSpecies()
    {}

    void GeneticSpecies::setBestIndividual(shared_ptr<GeneticIndividual> ind)
    {
        bestIndividualEver = ind;
        // Base improvement age off of best individual update.
        ageOfLastImprovement=age;
    }

    void GeneticSpecies::setMultiplier()
    {
        int dropoffAge = int(Globals::getSingleton()->getParameterValue("DropoffAge"));

        multiplier = 1;

        int ageSinceImprovement = age-ageOfLastImprovement;

        if (ageSinceImprovement>=dropoffAge)
        {
            multiplier *= 0.001;
        }

        //Give a fitness boost up to some young age (niching)
        //The age_significance parameter is a system parameter
        //  if it is 1, then young species get no fitness boost
        if (age<10)
            multiplier *= Globals::getSingleton()->getParameterValue("AgeSignificance");

        //Share fitness with the species
        multiplier /= currentIndividuals.size();
    }

    void GeneticSpecies::dump(TiXmlElement *speciesElement)
    {
        //Not implemented.  Not sure what I would want to dump.
    }

    void GeneticSpecies::setFitness()
    {
        speciesFitness=0;
        for (int a=0;a<(int)currentIndividuals.size();a++)
        {
            shared_ptr<GeneticIndividual> individual = currentIndividuals[a];

            speciesFitness+=individual->getFitness();
        }

        // Base improvement age off of average.  Rarely executed because average is always going up/down
        //if(speciesFitness/currentIndividuals.size() > oldAverageFitness)
        // ageSinceLastImprovement=age;

        oldAverageFitness = speciesFitness/currentIndividuals.size();
    }

    void GeneticSpecies::incrementAge()
    {
        age++;
        for (int a=0;a<(int)currentIndividuals.size();a++)
        {
            currentIndividuals[a]->incrementAge();
        }
    }

    void GeneticSpecies::makeBabies(vector<shared_ptr<GeneticIndividual> > &babies)
    {
        bool tournamentSelection = (Globals::getSingleton()->getParameterValue("TournamentSelection",0) > 0.5);
        GeneticIndividual::SingleFitnessDescendingComparator comparator;
        int lastIndex = int(Globals::getSingleton()->getParameterValue("SurvivalThreshold")*currentIndividuals.size());
        if( tournamentSelection || lastIndex >= currentIndividuals.size())
            lastIndex = currentIndividuals.size() - 1;

        /*
        cout << "species fitnesses: \n";
        for (int i=0; i<(int)currentIndividuals.size();i++)
        {
            cout << "............." << currentIndividuals[i]->getFitness() << "\n";
        }
        */

        for (int a=lastIndex+1;a<(int)currentIndividuals.size();a++)
        {
            //cout << "setting " << a << " to not be able to reproduce\n";
            currentIndividuals[a]->setCanReproduce(false);
        }

        double mutateOnlyProb = Globals::getSingleton()->getParameterValue("MutateOnlyProbability");
        for (int a=0;offspringCount>0;a++)
        {
            if (a>=1000000)
            {
                cout << "Error while making babies, need to choose the best of the species and bail!\n";
                //Something messed up, bail
                int parent = 0;
                shared_ptr<GeneticIndividual> ind = currentIndividuals[parent];
                babies.push_back(shared_ptr<GeneticIndividual>(new GeneticIndividual(ind,true)));
                ind->setHasReproduced(true);
                offspringCount--;
                continue;
            }

            bool onlyOneParent = (int(lastIndex)==0);
            if (onlyOneParent||Globals::getSingleton()->getRandom().getRandomDouble()<mutateOnlyProb)
            {
                int parent = Globals::getSingleton()->getRandom().getRandomWithinRange(0,int(lastIndex));
                shared_ptr<GeneticIndividual> ind = currentIndividuals[parent];
                if(tournamentSelection) {
                    vector<shared_ptr<GeneticIndividual> > tournament;
                    tournament.push_back(ind);
                    for(int i=1; i<Globals::getSingleton()->getParameterValue("TournamentSize",2); i++) {
                        parent = Globals::getSingleton()->getRandom().getRandomWithinRange(0,int(lastIndex));
                        tournament.push_back(currentIndividuals[parent]);
                    }
                    stable_sort(tournament.begin(), tournament.end(), comparator);
                    babies.push_back(shared_ptr<GeneticIndividual>(new GeneticIndividual(tournament[0],true)));
                    tournament[0]->setHasReproduced(true);
                } else {
                    babies.push_back(shared_ptr<GeneticIndividual>(new GeneticIndividual(ind,true)));
                    ind->setHasReproduced(true);
                }
                offspringCount--;
            }
            else
            {
                shared_ptr<GeneticIndividual> parent1,parent2;

                int parentIndex = Globals::getSingleton()->getRandom().getRandomWithinRange(0,int(lastIndex));
                parent1 = currentIndividuals[parentIndex];
                if(tournamentSelection) {
                    vector<shared_ptr<GeneticIndividual> > tournament;
                    tournament.push_back(parent1);
                    for(int i=1; i<Globals::getSingleton()->getParameterValue("TournamentSize",2); i++) {
                        parentIndex = Globals::getSingleton()->getRandom().getRandomWithinRange(0,int(lastIndex));
                        tournament.push_back(currentIndividuals[parentIndex]);
                    }
                    stable_sort(tournament.begin(), tournament.end(), comparator);
	                parent1 = tournament[0];
            	}

                int tt=0;
                do
                {
                    tt++;
                    if (tt==1000000)
                    {
                        cout << "Error while choosing parents.  Doing asexual reproduction\n";
                    }

                    parentIndex = Globals::getSingleton()->getRandom().getRandomWithinRange(0,int(lastIndex));
                    parent2 = currentIndividuals[parentIndex];
                    if(tournamentSelection) {
	                    vector<shared_ptr<GeneticIndividual> > tournament;
                        tournament.push_back(parent2);
                        for(int i=1; i<Globals::getSingleton()->getParameterValue("TournamentSize",2); i++) {
                            parentIndex = Globals::getSingleton()->getRandom().getRandomWithinRange(0,int(lastIndex));
                            tournament.push_back(currentIndividuals[parentIndex]);
                        }
                        stable_sort(tournament.begin(), tournament.end(), comparator);
                        parent2 = tournament[0];
                    }
                }
                while (parent2==parent1&&tt<=1000000);

                if (parent1==parent2)
                {
                    babies.push_back(shared_ptr<GeneticIndividual>(new GeneticIndividual(parent1,true)));
                    parent1->setHasReproduced(true);
                }
                else
                {
                    babies.push_back(shared_ptr<GeneticIndividual>(new GeneticIndividual(parent1,parent2)));
                    parent1->setHasReproduced(true);
                    parent2->setHasReproduced(true);
                }
                offspringCount--;
            }
        }
    }

}

