#ifndef __GENETICPOPULATION_H__
#define __GENETICPOPULATION_H__

#include "tinyxmldll.h"
#include "NEAT_STL.h"

#include "NEAT_Globals.h"
#include "NEAT_GeneticSpecies.h"
#include "NEAT_GeneticIndividual.h"



namespace NEAT
{

    /**
     * The Genetic Population class is responsible for holding and managing a population of individuals
     * over multiple generations.
     */
    class GeneticPopulation
    {
        vector<shared_ptr<GeneticGeneration> > generations;

        vector<shared_ptr<GeneticSpecies> > species;

        //we use a separate vector for extinct species to save CPU.
        vector<shared_ptr<GeneticSpecies> > extinctSpecies;

        int onGeneration;

        vector<vector<shared_ptr<GeneticIndividual> > > ranks;
        
        struct GenData {
			vector<pair<int, int> > mutations;
			vector<pair<pair<int, int>,pair<int, int> > > crossovers;
			vector<pair<int, int> > survivors;
		};
		
		map<int,GenData> genDataMap;
		map<shared_ptr<GeneticIndividual>, pair<int,int> > reproductionStats;
        

        struct FitnessComparator {
            int fitness;
            inline FitnessComparator(int fit) {
                fitness = fit;
            }

            inline bool operator() (shared_ptr<GeneticIndividual> i,shared_ptr<GeneticIndividual> j) {
              return (i->getFitnesses()[fitness] < j->getFitnesses()[fitness]);
            }
        };

        struct IndividualComparator {
            inline bool operator() (shared_ptr<GeneticIndividual> i,shared_ptr<GeneticIndividual> j) {
                if(i->getRank() != j->getRank()) {
                    return i->getRank() < j->getRank();
                } else {
                    return i->getCrowdingDistance() > j->getCrowdingDistance();
                }
            }
        };


    public:
        NEAT_DLL_EXPORT GeneticPopulation();

        NEAT_DLL_EXPORT GeneticPopulation(string fileName);

        NEAT_DLL_EXPORT virtual ~GeneticPopulation();

        inline shared_ptr<GeneticGeneration> getGeneration(int generationIndex=-1)
        {
            if (generationIndex==-1)
                generationIndex=int(onGeneration);

            return generations[generationIndex];
        }

        NEAT_DLL_EXPORT void addIndividual(shared_ptr<GeneticIndividual> individual);

        NEAT_DLL_EXPORT int getIndividualCount(int generation=-1);

        NEAT_DLL_EXPORT shared_ptr<GeneticIndividual> getIndividual(int a,int generation=-1);

        NEAT_DLL_EXPORT vector<shared_ptr<GeneticIndividual> >::iterator getIndividualIterator(int a,int generation=-1);

        NEAT_DLL_EXPORT shared_ptr<GeneticIndividual> getBestAllTimeIndividual();

        NEAT_DLL_EXPORT shared_ptr<GeneticIndividual> getBestIndividualOfGeneration(int generation=LAST_GENERATION);

        inline shared_ptr<GeneticSpecies> getSpecies(int id)
        {
            for (int a=0;a<(int)species.size();a++)
            {
                if (species[a]->getID()==id)
                    return species[a];
            }

            throw CREATE_LOCATEDEXCEPTION_INFO("Tried to get a species which doesn't exist (Maybe it went extinct?)");
        }

        NEAT_DLL_EXPORT void speciate();

        NEAT_DLL_EXPORT void setSpeciesMultipliers();

        NEAT_DLL_EXPORT void adjustFitness();

        NEAT_DLL_EXPORT void produceNextGeneration();

        NEAT_DLL_EXPORT void produceNextGenerationMultiObjective();

        NEAT_DLL_EXPORT void produceNextGenerationShadow();

        NEAT_DLL_EXPORT void rankByDominance();

        NEAT_DLL_EXPORT void calculateCrowdingDistances(vector<shared_ptr<GeneticIndividual> > front);

        NEAT_DLL_EXPORT void dump(string filename,bool includeGenes,bool doGZ);

        NEAT_DLL_EXPORT void dumpBest(string filename,bool includeGenes,bool doGZ);

        NEAT_DLL_EXPORT void dumpReproducedFromPreviousGeneration(string filename, bool includeGenes, bool doGZ);

        NEAT_DLL_EXPORT void cleanupOld(int generationSkip);

        inline int getGenerationCount()
        {
            return (int)generations.size();
        }



    };

}

#endif
