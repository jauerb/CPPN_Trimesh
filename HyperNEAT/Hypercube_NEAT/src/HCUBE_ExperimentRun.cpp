#include "HCUBE_ExperimentRun.h"

#include "Experiments/HCUBE_Experiment.h"

#include "Experiments/HCUBE_EvolveMorphTrimeshExperiment.h"
#include "Experiments/HCUBE_XorExperiment.h"
#include "Experiments/HCUBE_TicTacToeExperiment.h"
#include "Experiments/HCUBE_TicTacToeGameExperiment.h"
#include "Experiments/HCUBE_TicTacToeGameNoGeomExperiment.h"
#include "Experiments/HCUBE_FindPointExperiment.h"
#include "Experiments/HCUBE_FindClusterExperiment.h"
#include "Experiments/HCUBE_FindClusterNoGeomExperiment.h"
#include "Experiments/HCUBE_CheckersExperiment.h"

#include "Experiments/HCUBE_CheckersExperimentNoGeom.h"

#include "Experiments/HCUBE_CheckersExperimentOriginalFogel.h"


#ifndef HCUBE_NOGUI
#include "HCUBE_MainFrame.h"
#include "HCUBE_UserEvaluationFrame.h"

#include "HCUBE_ExperimentPanel.h"
#endif

#include "HCUBE_EvaluationSet.h"

namespace HCUBE
{
    ExperimentRun::ExperimentRun()
            :
            running(false),
            started(false),
            cleanup(false),
            populationMutex(new mutex()),
            frame(NULL)
    {
    }

    ExperimentRun::~ExperimentRun()
    {
        delete populationMutex;
    }

    void ExperimentRun::setupExperiment(
        int _experimentType,
        string _outputFileName
    )
    {
        experimentType = _experimentType;
        outputFileName = _outputFileName;

        cout << "SETTING UP EXPERIMENT TYPE: " << experimentType << endl;
		cout << "Using output file: " << outputFileName << endl;
		

		string experimentName = outputFileName.substr(0, outputFileName.find("."));
		
        switch (experimentType)
        {

        case EXPERIMENT_TIC_TAC_TOE:
            experiments.push_back(shared_ptr<Experiment>(new TicTacToeExperiment("")));
            break;
        case EXPERIMENT_TIC_TAC_TOE_GAME:
            experiments.push_back(shared_ptr<Experiment>(new TicTacToeGameExperiment("")));
            break;
        case EXPERIMENT_TIC_TAC_TOE_NO_GEOM_GAME:
            experiments.push_back(shared_ptr<Experiment>(new TicTacToeGameNoGeomExperiment("")));
            break;

        case EXPERIMENT_XOR:
            experiments.push_back(shared_ptr<Experiment>(new XorExperiment("")));
            break;

        case EXPERIMENT_FIND_POINT_EXPERIMENT:
            experiments.push_back(shared_ptr<Experiment>(new FindPointExperiment("")));
            break;
        case EXPERIMENT_FIND_CLUSTER_EXPERIMENT:
            experiments.push_back(shared_ptr<Experiment>(new FindClusterExperiment("")));
            break;
        case EXPERIMENT_FIND_CLUSTER_NO_GEOM_EXPERIMENT:
            experiments.push_back(shared_ptr<Experiment>(new FindClusterNoGeomExperiment("")));
            break;

        case EXPERIMENT_CHECKERS:
            experiments.push_back(shared_ptr<Experiment>(new CheckersExperiment("")));
            break;
        case EXPERIMENT_CHECKERS_NO_GEOM:
            experiments.push_back(shared_ptr<Experiment>(new CheckersExperimentNoGeom("")));
            break;

        case EXPERIMENT_CHECKERS_ORIGINAL_FOGEL:
            experiments.push_back(shared_ptr<Experiment>(new CheckersExperimentOriginalFogel("")));
            break;

        case EXPERIMENT_EVOLVE_MORPH_TRIMESH:
            experiments.push_back(shared_ptr<Experiment>(new EvolveMorphTrimeshExperiment(experimentName)));
            break;

        default:
            cout << string("ERROR: Unknown Experiment Type!\n");
            throw CREATE_LOCATEDEXCEPTION_INFO("ERROR: Unknown Experiment Type!");
        }
        for (int a=1;a<NUM_THREADS;a++)
        {
            experiments.push_back(shared_ptr<Experiment>(experiments[0]->clone()));
        }

    }

    void ExperimentRun::createPopulation(string populationString)
    {
        if (iequals(populationString,""))
        {
            int popSize = (int)NEAT::Globals::getSingleton()->getParameterValue("PopulationSize");
            population = shared_ptr<NEAT::GeneticPopulation>(
                             experiments[0]->createInitialPopulation(popSize)
                         );
        }
        else
        {


            population = shared_ptr<NEAT::GeneticPopulation>(
                             new NEAT::GeneticPopulation(populationString)
                         );
        }
    }

    void ExperimentRun::setupExperimentInProgress(
        string populationFileName,
        string _outputFileName
    )
    {
        outputFileName = _outputFileName;

        {
            TiXmlDocument *doc = new TiXmlDocument(populationFileName);
            
            cout << "Loading population\n";

            bool loadStatus;

            if (iends_with(populationFileName,".gz"))
            {
                loadStatus = doc->LoadFileGZ();
            }
            else
            {
                loadStatus = doc->LoadFile();
            }

            if (!loadStatus)
            {
                throw CREATE_LOCATEDEXCEPTION_INFO("Error trying to load the XML file!");
            }

            TiXmlElement *element = doc->FirstChildElement();

            NEAT::Globals* globals = NEAT::Globals::init(element);
            
            //Destroy the document
            delete doc;
            
            cout << "Population loaded\n";


        }


        int experimentType = int(NEAT::Globals::getSingleton()->getParameterValue("ExperimentType")+0.001);

        NEAT::Globals::getSingleton()->seedRandom((unsigned int)(NEAT::Globals::getSingleton()->getParameterValue("ActualRandomSeed")+0.001));

        cout << "Loading Experiment: " << experimentType << endl;

        setupExperiment(experimentType,_outputFileName);

        cout << "Experiment set up.  Creating population...\n";

        createPopulation(populationFileName);

        cout << "Population Created\n";
        
        /*cout << "Cleaning up old generations\n";
        
		population->cleanupOld(population->getGenerationCount()-1);
		
		cout << "Done cleaning\n";*/
		
    }

    void ExperimentRun::start()
    {
        if(true) {//NEAT::Globals::getSingleton()->getParameterValue("MultiObjective") > 0.5) {
            boost::filesystem::remove(outputFileName);
            boost::filesystem::remove(outputFileName+string(".gz"));
        }
        cout << "Experiment started\n";
#ifndef _DEBUG
        try
        {
#endif

            int maxGenerations = int(NEAT::Globals::getSingleton()->getParameterValue("MaxGenerations"));

            started=running=true;

            clock_t start,current,reference;
            start  = clock();
            reference = clock();

			
			//resuming an old experiment, want to produce next gen right away because last one already evaluated
			if ((population->getGenerationCount()-1) > 0) { 
                mutex::scoped_lock scoped_lock(*populationMutex);

                cout << "PRODUCING NEXT GENERATION\n";
                produceNextGeneration();
                cout << "DONE PRODUCING\n";							
			}
			
			int startGen = population->getGenerationCount()-1;
			
            for (int generations=(population->getGenerationCount()-1);generations<maxGenerations;generations++)
            {            
                current = clock();
                double time = (double(current)-double(reference))/CLOCKS_PER_SEC;
                if(time >= 3600.0 ) { //if at least an hour elapsed
                    double elapsed = (double(current)-double(start))/CLOCKS_PER_SEC;
                    printf("TIME_STATS: best fitness = %5.5f, generation = %d, elapsed time = %5.5f s\n", population->getBestAllTimeIndividual()->getFitness(), generations - 1,elapsed);
                    reference = clock();
                }

                if (generations>startGen)
                {
                    mutex::scoped_lock scoped_lock(*populationMutex);
                    //set IDs before reproducing so maintain ancestry -- NO LONGER NEEDED SINCE IDs will have been set in finishEvaluations
					//population->getGeneration()->setIDs();

                    cout << "PRODUCING NEXT GENERATION\n";
                    produceNextGeneration();
                    cout << "DONE PRODUCING\n";

//                    cout << "DUMPING REPRODUCED FROM PREVIOUS GENERATION\n";
//                    population->dumpReproducedFromPreviousGeneration(outputFileName/*+string(".backup.xml")*/,true,true);
//                    cout << "DONE DUMPING\n";
                }

                if (experiments[0]->performUserEvaluations())
                {
#ifdef HCUBE_NOGUI
                    throw CREATE_LOCATEDEXCEPTION_INFO("ERROR: TRIED TO USE INTERACTIVE EVOLUTION WITH NO GUI!");
#else
                    frame->getUserEvaluationFrame()->updateEvaluationPanels();
                    running=false;
                    while (!running)
                    {
                        boost::xtime xt;
                        boost::xtime_get(&xt, boost::TIME_UTC);
                        xt.sec += 1;
                        boost::thread::sleep(xt); // Sleep for 1/2 second
                        //cout << "Sleeping while user evaluates!\n";
                    }
#endif
                }
                else
                {
                    while (!running)
                    {
                        boost::xtime xt;
                        boost::xtime_get(&xt, boost::TIME_UTC);
                        xt.sec += 1;
                        boost::thread::sleep(xt); // Sleep for 1/2 second
                    }
                    evaluatePopulation();
                }

                cout << "Finishing evaluations\n";
                finishEvaluations();
                cout << "Evaluations Finished\n";
            }

            //if want to dump all will be taken care of from finishEvaluations now for all gens
            //cout << "DUMPING ALL INDIVIDUALS FROM FINAL GENERATION\n";
            //population->getGeneration()->setIDs();
            //population->dump(outputFileName/*+string(".backup.xml")*/,true,true);
            //cout << "DONE DUMPING\n";

            cout << "Experiment finished\n";

            //cout << "Saving Dump...";
            //population->dump(outputFileName,true,false);
            //cout << "Done!\n";


            if(true) {//NEAT::Globals::getSingleton()->getParameterValue("MultiObjective") > 0.5) {
                //need to combine files into one
                //TiXmlDocument doc();
                TiXmlElement *root = new TiXmlElement("Genetics");
                NEAT::Globals::getSingleton()->dump(root);


                /*
                stringstream ss;
                //root->Print(ss,0);


                ss << root;
                string s = ss.str();

                */
                cout << "Merging Output Files...";

                TiXmlPrinter rootPrinter;
                root->Accept( &rootPrinter );
                string s = rootPrinter.CStr();

                ofstream out( outputFileName.c_str() );
                string lastGenFileName = outputFileName + string(".lastgen.xml");
                ofstream out2( lastGenFileName.c_str() );

                out << s.substr(0, s.length() - 3) << ">" <<endl;
                out2 << s.substr(0, s.length() - 3) << ">" <<endl;

                int maxGenerations = int(NEAT::Globals::getSingleton()->getParameterValue("MaxGenerations"));
                for(int i=0; i<maxGenerations; i++) {
                    stringstream ss;
                    ss << i;
                    TiXmlDocument doc( outputFileName + string("-") + ss.str() + string(".backup.xml.gz") );
                    doc.LoadFileGZ();
                    TiXmlPrinter printer;
                    doc.Accept(&printer);
                    out << printer.CStr() << endl;
                    if( i == (maxGenerations - 1))
                        out2 << printer.CStr() << endl;
                }
                out << "</Genetics>" << endl;
                out2 << "</Genetics>" << endl;
                out.close();
                out2.close();
                cout << "Done!\n";
                cout << "Compressing Merged File...";
                stringstream ss;
                ss << "gzip " << outputFileName;
                std::system(ss.str().c_str());
                stringstream ssLastGen;
                ssLastGen << "gzip " << lastGenFileName;
                std::system(ssLastGen.str().c_str());

                cout << "Done!\n";
                cout << "Deleting backup files...";
                for(int i=0; i<maxGenerations; i++) {
                    stringstream ss2;
                    ss2 << outputFileName << "-" << i << ".backup.xml.gz";
                    boost::filesystem::remove(ss2.str());
                }
                boost::filesystem::remove(outputFileName + string("-root.backup.xml"));
                cout << "Done!\n";


            } else {
                cout << "Saving best individuals...";
                string bestFileName = outputFileName.substr(0,outputFileName.length()-4)+string("_best.xml");
                population->dumpBest(bestFileName,true,true);
                cout << "Done!\n";

                cout << "Deleting backup file...";
                boost::filesystem::remove(outputFileName+string(".backup.xml"));
                cout << "Done!\n";
            }
#ifndef _DEBUG
        }
        catch (const std::exception &ex)
        {
			cout << "CAUGHT ERROR AT " << __FILE__ << " : " << __LINE__ << endl;
            CREATE_PAUSE(ex.what());
        }
        catch (...)
        {
			cout << "CAUGHT ERROR AT " << __FILE__ << " : " << __LINE__ << endl;
            CREATE_PAUSE("AN UNKNOWN EXCEPTION HAS BEEN THROWN!");
        }
#endif
    }

    void ExperimentRun::evaluatePopulation()
    {
    	cout << "Evaluating Population\n";
        shared_ptr<NEAT::GeneticGeneration> generation = population->getGeneration();
        //Randomize population order for evaluation
        generation->randomizeIndividualOrder();

        int populationSize = population->getIndividualCount();

        int populationPerProcess = populationSize/NUM_THREADS;

        boost::thread** threads = new boost::thread*[NUM_THREADS];
        EvaluationSet** evaluationSets = new EvaluationSet*[NUM_THREADS];

        for(int i = 0; i < experiments.size(); i++) {
            experiments[i]->setNumber(i + 1);
        }

		cout << "Creating Evaluation Sets\n";
        for (int i = 0; i < NUM_THREADS; ++i)
        {
            if (i+1==NUM_THREADS)
            {
                //Fix for uneven distribution
                int populationIteratorSize =
                    populationSize
                    - populationPerProcess*(NUM_THREADS-1);
                evaluationSets[i] =
                    new EvaluationSet(
                    experiments[i],
                    generation,
                    population->getIndividualIterator(populationPerProcess*i),
                    populationIteratorSize
                );
            }
            else
            {

                evaluationSets[i] =
                    new EvaluationSet(
                    experiments[i],
                    generation,
                    population->getIndividualIterator(populationPerProcess*i),
                    populationPerProcess
                );
            }
			cout << "set " << i << "created, creating thread\n";
            threads[i] =
                new boost::thread(
                boost::bind(
                    &EvaluationSet::run,
                    evaluationSets[i]
                )
            );
            cout << "thread created\n";
        }

        //loop through each thread, making sure it is finished before we move on
        for (int i=0;i<NUM_THREADS;++i)
        {
            /*if (!evaluationSets[i]->isFinished())
            {
            --i;
            boost::xtime xt;
            boost::xtime_get(&xt, boost::TIME_UTC);
            xt.sec += 1;
            boost::thread::sleep(xt); // Sleep for 1/2 second
            }*/
            threads[i]->join();
        }

        for (int i = 0; i < NUM_THREADS; ++i)
        {
            delete threads[i];
            delete evaluationSets[i];
        }

		delete[] threads;
		delete[] evaluationSets;
    }

    void ExperimentRun::finishEvaluations()
    {
        cout << "Cleaning up...\n";
        //int generationDumpModulo = int(NEAT::Globals::getSingleton()->getParameterValue("GenerationDumpModulo"));
        if (cleanup)
            population->cleanupOld(INT_MAX/2);

        cout << "Adjusting fitness...\n";
        population->adjustFitness();

        population->getGeneration()->setIDs();
        if((NEAT::Globals::getSingleton()->getParameterValue("DumpAll",0.0) > 0.5) || (population->getGenerationCount() % 50 == 0) ) {
            cout << "Dumping individuals...\n";
            population->dump(outputFileName/*+string(".backup.xml")*/,true,true);
            
            TiXmlElement *root = new TiXmlElement("Genetics");
		    NEAT::Globals::getSingleton()->dump(root);
		    TiXmlPrinter rootPrinter;
			root->Accept( &rootPrinter );
		    string s = rootPrinter.CStr();
		    string rootFileName = outputFileName + string("-root.backup.xml");
		    ofstream out( rootFileName.c_str() );
		    out << s.substr(0, s.length() - 3) << ">" <<endl;
		    out.close();

        } else {
            cout << "Dumping best individuals...\n";
            //population->dumpBest(outputFileName+string(".backup.xml"),true,true);
            population->dumpBest(outputFileName/*+string(".backup.xml")*/,true,true);

            //population->cleanupOld(25);
            //population->dumpBest("out/dumpBestWithGenes(backup).xml",true);
        }
        cout << "Done Dumping\n";

#ifndef HCUBE_NOGUI
        if (frame)
        {
            frame->updateNumGenerations(population->getGenerationCount());
        }
#endif

        cout << "Resetting generation data...\n";
        shared_ptr<NEAT::GeneticGeneration> generation = population->getGeneration();
        experiments[0]->resetGenerationData(generation);

        for (int a=0;a<population->getIndividualCount();a++)
        {
            //cout << __FILE__ << ":" << __LINE__ << endl;
            experiments[0]->addGenerationData(generation,population->getIndividual(a));
        }

        /*
        if(experimentType == EXPERIMENT_TIC_TAC_TOE_GAME)
        {
        //Take the best individual and run him a lot

        ((TicTacToeGameExperiment*)experiment)->setNumGames(20000);
        ((TicTacToeGameExperiment*)experiment)->setNumGames(100);
        }
        */
        
    }

    void ExperimentRun::produceNextGeneration()
    {
        cout << "Producing next generation.\n";
        try
        {
            population->produceNextGeneration();
        }
        catch (const std::exception &ex)
        {
            cout << "EXCEPTION DURING POPULATION REPRODUCTION: " << endl;
            CREATE_PAUSE(ex.what());
        }
        catch (...)
        {
            cout << "Unhandled Exception\n";
        }
    }


}

