#include "HCUBE_Defines.h"

#include "JGTL_CommandLineParser.h"

#ifndef HCUBE_NOGUI
# include "HCUBE_MainApp.h"
#endif

#include "HCUBE_ExperimentRun.h"

#include "Experiments/HCUBE_Experiment.h"
#include "Experiments/HCUBE_FindClusterExperiment.h"
#include "Experiments/HCUBE_CheckersExperiment.h"
#include "Experiments/HCUBE_EvolveMorphTrimeshExperiment.h"




#ifndef HCUBE_NOGUI
namespace HCUBE
{
    IMPLEMENT_APP_NO_MAIN(MainApp)
}
#endif

#define EPOCHS_PER_PRINT (100000)

void printNetwork(NEAT::FastNetwork<double> &testNetwork)
{
    return;

    cout << "Network links:\n";
    cout << testNetwork.getLink("Input0","Hidden1")->weight << endl;
    cout << testNetwork.getLink("Input0","Hidden2")->weight << endl;
    cout << endl;

    cout << testNetwork.getLink("Input1","Hidden1")->weight << endl;
    cout << testNetwork.getLink("Input1","Hidden2")->weight << endl;
    cout << endl;

    cout << testNetwork.getLink("Input2","Hidden1")->weight << endl;
    cout << testNetwork.getLink("Input2","Hidden2")->weight << endl;
    cout << endl;

    cout << testNetwork.getLink("Hidden0","Output0")->weight << endl;
    cout << testNetwork.getLink("Hidden1","Output0")->weight << endl;
    cout << testNetwork.getLink("Hidden2","Output0")->weight << endl;
    cout << endl;

    CREATE_PAUSE("");
}

int HyperNEAT_main(int argc,char **argv)
{
    CommandLineParser commandLineParser(argc,argv);

#if 0
    NEAT::Globals::init();

    NEAT::Globals::getSingleton()->setParameterValue("SignedActivation",0.0);

    vector<NEAT::NetworkNode*> nodes;
    vector<NEAT::NetworkLink*> links;


    nodes.push_back(new NEAT::NetworkNode("Input0",false));
    nodes.push_back(new NEAT::NetworkNode("Input1",false));
    nodes.push_back(new NEAT::NetworkNode("Input2",false));

    nodes.push_back(new NEAT::NetworkNode("Hidden0",false));
    nodes.push_back(new NEAT::NetworkNode("Hidden1",true));
    nodes.push_back(new NEAT::NetworkNode("Hidden2",true));

    nodes.push_back(new NEAT::NetworkNode("Output0",true));

    links.push_back(new NEAT::NetworkLink(nodes[0],nodes[4],true,0.1));
    links.push_back(new NEAT::NetworkLink(nodes[0],nodes[5],true,0.3));
    links.push_back(new NEAT::NetworkLink(nodes[1],nodes[4],true,0.2));
    links.push_back(new NEAT::NetworkLink(nodes[1],nodes[5],true,0.2));
    links.push_back(new NEAT::NetworkLink(nodes[2],nodes[4],true,-0.4));
    links.push_back(new NEAT::NetworkLink(nodes[2],nodes[5],true,-0.1));

    links.push_back(new NEAT::NetworkLink(nodes[3],nodes[6],true,0.1));
    links.push_back(new NEAT::NetworkLink(nodes[4],nodes[6],true,0.2));
    links.push_back(new NEAT::NetworkLink(nodes[5],nodes[6],true,0.3));

    NEAT::FastNetwork<double> testNetwork(nodes,links);

    for(int epoch=0;;epoch++)
    {

    testNetwork.setValue("Input0",1);
    testNetwork.setValue("Input1",0);
    testNetwork.setValue("Input2",0);
    testNetwork.setValue("Hidden0",1);

    testNetwork.dummyActivation();
    testNetwork.update(2);

    if(epoch%EPOCHS_PER_PRINT==0)
        cout << "ERRORs: " << fabs(testNetwork.getValue("Output0") - 0.0) << '\t';

    {
        vector<string> nodeNames;
        vector<double> correctedValues;

        nodeNames.push_back("Output0");
        correctedValues.push_back(0.0);

        testNetwork.backProp(nodeNames,correctedValues);
    }

    if(fabs(testNetwork.getValue("Output0") - 0.0) < 0.3)
    {
        printNetwork(testNetwork);
    }

    testNetwork.reinitialize();

    testNetwork.setValue("Input0",1);
    testNetwork.setValue("Input1",0);
    testNetwork.setValue("Input2",1);
    testNetwork.setValue("Hidden0",1);

    testNetwork.dummyActivation();
    testNetwork.update(2);

    if(epoch%EPOCHS_PER_PRINT==0)
        cout << fabs(testNetwork.getValue("Output0") - 1.0) << '\t';

    {
        vector<string> nodeNames;
        vector<double> correctedValues;

        nodeNames.push_back("Output0");
        correctedValues.push_back(1.0);

        testNetwork.backProp(nodeNames,correctedValues);
    }

    if(fabs(testNetwork.getValue("Output0") - 1.0) < 0.3)
    {
        printNetwork(testNetwork);
    }

    testNetwork.reinitialize();

    testNetwork.setValue("Input0",1);
    testNetwork.setValue("Input1",1);
    testNetwork.setValue("Input2",0);
    testNetwork.setValue("Hidden0",1);

    testNetwork.dummyActivation();
    testNetwork.update(2);

    if(epoch%EPOCHS_PER_PRINT==0)
        cout << fabs(testNetwork.getValue("Output0") - 1.0) << '\t';

    {
        vector<string> nodeNames;
        vector<double> correctedValues;

        nodeNames.push_back("Output0");
        correctedValues.push_back(1.0);

        testNetwork.backProp(nodeNames,correctedValues);
    }

    if(fabs(testNetwork.getValue("Output0") - 1.0) < 0.3)
    {
        printNetwork(testNetwork);
    }

    testNetwork.reinitialize();

    testNetwork.setValue("Input0",1);
    testNetwork.setValue("Input1",1);
    testNetwork.setValue("Input2",1);
    testNetwork.setValue("Hidden0",1);

    testNetwork.dummyActivation();
    testNetwork.update(2);

    if(epoch%EPOCHS_PER_PRINT==0)
        cout << fabs(testNetwork.getValue("Output0") - 0.0) << endl;

    {
        vector<string> nodeNames;
        vector<double> correctedValues;

        nodeNames.push_back("Output0");
        correctedValues.push_back(0.0);

        testNetwork.backProp(nodeNames,correctedValues);
    }

    if(fabs(testNetwork.getValue("Output0") - 0.0) < 0.3)
    {
        printNetwork(testNetwork);
    }
    }

    NEAT::Globals::deinit();
#endif

#if 1
    //This is for memory debugging
    //_CrtSetBreakAlloc(137405);

    try
    {
		if(commandLineParser.HasSwitch("-P")) {
            HCUBE::ExperimentRun experimentRun;

            experimentRun.setupExperimentInProgress(commandLineParser.GetSafeArgument("-I",0,"input.xml"),  "" );
		    HCUBE::Experiment *experiment = experimentRun.getExperiment()->clone();


            cout << "Experiment set up\n";
			if(commandLineParser.HasSwitch("-AllBest")) {
				cout << "Getting best guy from each gen\n";
				cout << experimentRun.getPopulation()->getGenerationCount() << endl;
		    	for(int gen=0; gen<experimentRun.getPopulation()->getGenerationCount(); gen++) {
			    	shared_ptr<NEAT::GeneticGeneration> generation = experimentRun.getGeneration(gen);
			    	generation->sortByFitness();
			    	shared_ptr<NEAT::GeneticIndividual> indiv =
				        shared_ptr<NEAT::GeneticIndividual>(
				            new NEAT::GeneticIndividual(
				                *(generation->getIndividual(0).get())
				            )
				        );
   				    cout << "Gen " << gen << ", Recorded fitness " << indiv->getFitness() << "\n";
					if(commandLineParser.HasSwitch("-R") && experimentRun.getExperimentType() == EXPERIMENT_EVOLVE_MORPH_TRIMESH)
	   				    ((HCUBE::EvolveMorphTrimeshExperiment*) experiment)->processIndividualPostHoc(indiv,stringTo<int>(commandLineParser.GetSafeArgument("-R",0,"0")));
	   				else
						experiment->processIndividualPostHoc(indiv);		    	
		    	}
		    } else {
			    int gen;
			    if(commandLineParser.HasSwitch("-G"))
			        gen = stringTo<int>(commandLineParser.GetSafeArgument("-G",0,"0"));
			    else
			        gen= experimentRun.getPopulation()->getGenerationCount() - 1;

			    int num;
			    if(commandLineParser.HasSwitch("-N")) {
			        num = stringTo<int>(commandLineParser.GetSafeArgument("-N",0,"0"));
			        if(commandLineParser.HasSwitch("-A"))
			        	cout << "Top " << num << " individuals from generation " << gen << "\n";
			        else
				        cout << "Individual " << num << " from generation " << gen << "\n";
			    } else {
				    num= 0;
				    cout << "Best Individual from generation " << gen << "\n";
		    	}

				if(commandLineParser.HasSwitch("-A")) {
					//don't assume sorted... get top num guys
					shared_ptr<NEAT::GeneticGeneration> generation = experimentRun.getGeneration(gen);
					generation->sortByFitness();
					for(int i=0; i<num; i++) {
						shared_ptr<NEAT::GeneticIndividual> indiv =
						    shared_ptr<NEAT::GeneticIndividual>(
						        new NEAT::GeneticIndividual(
						            *(generation->getIndividual(i).get())
						        )
						    );
						cout << "Recorded fitness " << indiv->getFitness() << "\n";

			    		if(commandLineParser.HasSwitch("-R") && experimentRun.getExperimentType() == EXPERIMENT_EVOLVE_MORPH_TRIMESH)
		   				    ((HCUBE::EvolveMorphTrimeshExperiment*) experiment)->processIndividualPostHoc(indiv,stringTo<int>(commandLineParser.GetSafeArgument("-R",0,"0")));
						else
							experiment->processIndividualPostHoc(indiv);

				    }	    	
		    	
	    		} else {

					shared_ptr<NEAT::GeneticIndividual> indiv =
						    shared_ptr<NEAT::GeneticIndividual>(
						        new NEAT::GeneticIndividual(
						            *(experimentRun.getIndividual(gen,num).get())
						        )
						    );
				    cout << "Recorded fitness " << indiv->getFitness() << "\n";

					experiment->processIndividualPostHoc(indiv);
					
	    		}
    		}
		    delete experiment;



		} else if (argc<=1) {
#ifndef HCUBE_NOGUI
            cout << "Starting wxEntry...\n";
            return wxEntry(argc, argv);
#else
            cout << "You must pass the parameters and the output file as command "
            << "parameters!\n";
            cout << "(If you intended to run in GUI mode, please run cmake and set the USE_GUI flag to true)\n";
#endif
        }
        else if (argc==2)
        {
            //Run the post-hoc analysis on every generation
            HCUBE::ExperimentRun experimentRun;

            experimentRun.setupExperimentInProgress(
                string(argv[1]),
                ""
            );

            int numGenerations = experimentRun.getPopulation()->getGenerationCount();

            HCUBE::Experiment *experiment = experimentRun.getExperiment()->clone();

            /*
            {
                string outString = (erase_tail_copy(string(argv[1]),4)+string("_fitness.out"));
                cout << "Creating file " << outString << endl;
                ofstream outfile( outString.c_str() );
                string previousSummary;

                bool doOnce=false;

                for (int generation=0;generation<numGenerations;generation++)
                {
                    //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));

                    //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));

                    if ( generation &&
                            (*(experimentRun.getIndividual(generation,0).get())) == (*(experimentRun.getIndividual(generation-1,0).get())) )
                    {
                        outfile << (generation+1) << ' ' << previousSummary << endl;
                        continue;
                    }

                    shared_ptr<NEAT::GeneticIndividual> indiv =
                        shared_ptr<NEAT::GeneticIndividual>(
                            new NEAT::GeneticIndividual(
                                *(experimentRun.getIndividual(generation,0).get())
                            )
                        );

                    //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));

                    cout << "Beginning fitness evaluation " << (generation+1) << "/" << numGenerations << "...";
                    experiment->addIndividualToGroup(indiv);
                    experiment->processGroup(experimentRun.getGeneration(generation));
                    experiment->clearGroup();
                    cout << "done!\n";

                    //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));

                    if (indiv->getUserData())
                    {
                        if (!doOnce)
                        {
                            doOnce=true;
                            outfile
                            << "#Generation: "
                            << indiv->getUserData()->summaryHeaderToString()
                            << endl;
                        }

                        previousSummary = indiv->getUserData()->summaryToString();
                        outfile << (generation+1) << ' ' << previousSummary << endl;
                    }
                    else
                    {
                        throw CREATE_LOCATEDEXCEPTION_INFO("No user data!\n");
                    }

                }
            }
            */

            {
                string outString = (erase_tail_copy(string(argv[1]),3)+string("_fitness.out"));
                cout << "Creating file " << outString << endl;
                ofstream outfile( outString.c_str() );
                string previousSummary;

                bool doOnce=false;

                for (int generation=0;generation<numGenerations;generation++)
                {
                    //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));

                    //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));

                    if ( generation &&
                            (*(experimentRun.getIndividual(generation,0).get())) == (*(experimentRun.getIndividual(generation-1,0).get())) )
                    {
                        outfile << (generation+1) << ' ' << previousSummary << endl;
                        continue;
                    }

                    shared_ptr<NEAT::GeneticIndividual> indiv =
                        shared_ptr<NEAT::GeneticIndividual>(
                            new NEAT::GeneticIndividual(
                                *(experimentRun.getIndividual(generation,0).get())
                            )
                        );

                    //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));

                    //cout << "Beginning post-hoc evaluation " << (generation+1) << "/" << numGenerations << "...";
                    //experiment->processIndividualPostHoc(indiv);
                    //cout << "done!\n";

                    //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));

                    if (indiv->getUserData())
                    {
                        if (!doOnce)
                        {
                            doOnce=true;
                            outfile
                            << "#Generation: "
                            << indiv->getUserData()->summaryHeaderToString()
                            << endl;
                        }

                        previousSummary = indiv->getUserData()->summaryToString();
                        outfile << (generation+1) << ' ' << previousSummary << endl;
                    }
                    else
                    {
                        if (!doOnce)
                        {
                            doOnce=true;
                            outfile
                            << "#Generation: "
                            << "Fitness:"
                            << endl;
                        }

                        previousSummary = toString(indiv->getFitness());
                        outfile << (generation+1) << ' ' << previousSummary << endl;
                    }

                }
            }

#if 0
                    cout << "Beginning post-hoc evaluation " << (generation+1) << "/" << numGenerations << "...";
                    experiment->processIndividualPostHoc(indiv);
                    cout << "done!\n";

                    //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));

                    if (indiv->getUserData())
                    {
                        if (!doOnce)
                        {
                            doOnce=true;
                            outfile
                            << "#Generation: "
                            << indiv->getUserData()->summaryHeaderToString()
                            << endl;
                        }

                        previousSummary = indiv->getUserData()->summaryToString();
                        outfile << (generation+1) << ' ' << previousSummary << endl;
                    }
                    else
                    {
                        if (!doOnce)
                        {
                            doOnce=true;
                            outfile
                            << "#Generation: "
                            << "Fitness:"
                            << endl;
                        }

                        previousSummary = toString(indiv->getFitness());
                        outfile << (generation+1) << ' ' << previousSummary << endl;
                    }

                }
            }
#endif

            /*
            cout << "Done with run!  Running medium resolution tests...\n";
            ((HCUBE::FindClusterExperiment*)experiment)->increaseResolution();

            {
                string outString = (erase_tail_copy(string(argv[1]),4)+string("_midres.out"));
                ofstream outfile( outString.c_str() );
                cout << "Creating file " << outString << endl;
                string previousSummary;

                bool doOnce=false;

                for (int generation=0;generation<numGenerations;generation++)
                {
                    //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));

                    //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));

                    if ( generation &&
                            (*(experimentRun.getIndividual(generation,0).get())) == (*(experimentRun.getIndividual(generation-1,0).get())) )
                    {
                        outfile << (generation+1) << ' ' << previousSummary << endl;
                        continue;
                    }

                    shared_ptr<NEAT::GeneticIndividual> indiv =
                        shared_ptr<NEAT::GeneticIndividual>(
                            new NEAT::GeneticIndividual(
                                *(experimentRun.getIndividual(generation,0).get())
                            )
                        );

                    //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));

                    cout << "Beginning post-hoc evaluation " << (generation+1) << "/" << numGenerations << "...";
                    experiment->processIndividualPostHoc(indiv);
                    cout << "done!\n";

                    //CREATE_PAUSE(string("Error!: ")+toString(__LINE__));

                    if (indiv->getUserData())
                    {
                        if (!doOnce)
                        {
                            doOnce=true;
                            outfile
                            << "#Generation: "
                            << indiv->getUserData()->summaryHeaderToString()
                            << endl;
                        }

                        previousSummary = indiv->getUserData()->summaryToString();
                        outfile << (generation+1) << ' ' << previousSummary << endl;
                    }

                }
            }
            */
        }
        else if(commandLineParser.HasSwitch("-I") && commandLineParser.HasSwitch("-O")) {
            NEAT::Globals::init(commandLineParser.GetSafeArgument("-I",0,"input.dat"));

            if(commandLineParser.HasSwitch("-R"))
            {
                NEAT::Globals::getSingleton()->seedRandom(stringTo<unsigned int>(commandLineParser.GetSafeArgument("-R",0,"0")));
            }

            int experimentType = int(NEAT::Globals::getSingleton()->getParameterValue("ExperimentType")+0.001);

            cout << "Loading Experiment: " << experimentType << endl;

            HCUBE::ExperimentRun experimentRun;

            experimentRun.setupExperiment(experimentType,commandLineParser.GetSafeArgument("-O",0,"output.xml"));

            cout << "Experiment set up\n";

            experimentRun.createPopulation();

            experimentRun.setCleanup(true);

            cout << "Population Created\n";

            experimentRun.start();
        }
        else if(commandLineParser.HasSwitch("-Y") && commandLineParser.HasSwitch("-O")) {
                    HCUBE::ExperimentRun experimentRun;

            experimentRun.setupExperimentInProgress(commandLineParser.GetSafeArgument("-Y",0,"input.xml"),  commandLineParser.GetSafeArgument("-O",0,"output.xml"));
            
            experimentRun.setCleanup(true);
            
            experimentRun.start();
        
        }        
        else
        {
            cout << "Syntax for passing command-line options to HyperNEAT (do not actually type '(' or ')' ):\n";
            cout << "./HyperNEAT [-R (seed)] -I (datafile) -O (outputfile)\n";
        }
    }
    catch (string s)
    {
		cout << "CAUGHT ERROR AT " << __FILE__ << " : " << __LINE__ << endl;
        cout << "An exception has occured: " << s << endl;
    }
    catch (...)
    {
		cout << "CAUGHT ERROR AT " << __FILE__ << " : " << __LINE__ << endl;
        cout << "An error has occured!\n";
    }

    NEAT::Globals::deinit();

#endif
    return 0;
}

int main(int argc,char **argv)
{
    HyperNEAT_main(argc,argv);
}
