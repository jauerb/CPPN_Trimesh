#include "Experiments/HCUBE_EvolveMorphTrimeshExperiment.h"

#include <stdio.h>

#include <iostream>
#include "fstream"

#include <map>
#include <queue>
#include <ctime>

#include <algorithm>

#define PRINT_DEBUG_MSGS 0
#define DUMP_ALL 0


namespace HCUBE
{
    using namespace NEAT;
    EvolveMorphTrimeshExperiment::EvolveMorphTrimeshExperiment(string _experimentName):Experiment(_experimentName)
    {
        randSeed = Globals::getSingleton()->getRandom().getSeed();
        if(randSeed > 10000)
            randSeed = 1;
    }

    EvolveMorphTrimeshExperiment::~EvolveMorphTrimeshExperiment()
    {
        results.clear();
    }


    NEAT::GeneticPopulation* EvolveMorphTrimeshExperiment::createInitialPopulation(int populationSize)
    {
        cout << "CREATING POPULATION for EvolveMorphTrimeshExperiment\n";
        NEAT::GeneticPopulation* population = new NEAT::GeneticPopulation();

        vector<GeneticNodeGene> genes;

        genes.push_back(GeneticNodeGene("Bias","NetworkSensor",0,false));

        genes.push_back(GeneticNodeGene("X1","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("Y1","NetworkSensor",0,false));
        genes.push_back(GeneticNodeGene("Z1","NetworkSensor",0,false));

        genes.push_back(GeneticNodeGene("Matter","NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));

		int numDOF = 2;
        for (int a=0;a<populationSize;a++)
        {
            //cout << "adding params for individual " << a << "\n";
            shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,true,1.0));
		    for(int i=1; i<=numDOF; i++) {
			    stringstream ss;
			    ss << i;	    
	            individual->addParam(GeneticParamGene("Amplitude" + ss.str(), 0.25,0.75));
	            individual->addParam(GeneticParamGene("Period" + ss.str(), 250.0, 1500.0));
			    individual->addParam(GeneticParamGene("PhaseShift" + ss.str(), -1.0, 1.0)); //fraction of period
	    	
	    	}
	    	//in other experiments these were evolved, but not here (hence min=max)
	    	//left as params because simulator looks for these values in params file
		    individual->addParam(GeneticParamGene("Joint1X", 1.0, 1.0));
            individual->addParam(GeneticParamGene("Joint1Y", 0.0, 0.0));
            individual->addParam(GeneticParamGene("Joint1Z", 0.0, 0.0));
            individual->addParam(GeneticParamGene("Joint2X", 0.0, 0.0));
            individual->addParam(GeneticParamGene("Joint2Y", 0.0, 0.0));
            individual->addParam(GeneticParamGene("Joint2Z", -1.0, -1.0));
	
	        population->addIndividual(individual);
    	}

        return population;
    }

    void EvolveMorphTrimeshExperiment::processGroup(shared_ptr<NEAT::GeneticGeneration> generation) {
        sprintf(tempFileName1, "%s/Files%d_%d/temp1.dat",BASE_DIR,randSeed, number);
        sprintf(tempFileName2, "%s/Files%d_%d/temp2.dat",BASE_DIR,randSeed, number);
        sprintf(inFileName, "%s/Files%d_%d/Results.dat",BASE_DIR,randSeed, number);
        sprintf(verticesFileName, "%s/Files%d_%d/Vertices.dat",BASE_DIR,randSeed, number);
        sprintf(indicesFileName, "%s/Files%d_%d/Indices.dat",BASE_DIR,randSeed, number);
        sprintf(paramsFileName, "%s/Files%d_%d/Params.dat",BASE_DIR,randSeed, number);
        sprintf(readyFileName, "%s/Files%d_%d/Ready.dat",BASE_DIR,randSeed, number);

        shared_ptr<NEAT::GeneticIndividual> individual = group.front();
        if(individual->getFitnesses().size() == 0) {
            EvaluateIndividual(individual, false);

        }
    }

    void EvolveMorphTrimeshExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual) {
	    processIndividualPostHoc(individual, 1);
    }

    void EvolveMorphTrimeshExperiment::processIndividualPostHoc(shared_ptr<NEAT::GeneticIndividual> individual, int useSubDir) {
        sprintf(tempFileName1, "%s/Files%d/temp1.dat",BASE_DIR,useSubDir);
        sprintf(tempFileName2, "%s/Files%d/temp2.dat",BASE_DIR,useSubDir);
        sprintf(inFileName, "%s/Files%d/Results.dat",BASE_DIR,useSubDir);
        sprintf(verticesFileName, "%s/Files%d/Vertices.dat",BASE_DIR,useSubDir);
        sprintf(indicesFileName, "%s/Files%d/Indices.dat",BASE_DIR,useSubDir);
        sprintf(paramsFileName, "%s/Files%d/Params.dat",BASE_DIR,useSubDir);
        sprintf(readyFileName, "%s/Files%d/Ready.dat",BASE_DIR,useSubDir);
        EvaluateIndividual(individual, true);
    }

/*
   Given grid cells and "matter" values calculate the triangular facets.
   Returns a vector of triangles.
   
   Adapted from http://paulbourke.net/geometry/polygonise/
*/
    vector<EvolveMorphTrimeshExperiment::Triangle> EvolveMorphTrimeshExperiment::Polygonise(vector<EvolveMorphTrimeshExperiment::Coordinates> gridVertices, vector<double> values, double isolevel) {
       int i,ntriang;
       int cubeindex;
       Coordinates vertlist[12];
       vector<Triangle> triangles;

       /*
          Determine the index into the edge table which
          tells us which vertices are inside of the surface
       */
       cubeindex = 0;
       if (values[0] < isolevel) cubeindex |= 1;
       if (values[1] < isolevel) cubeindex |= 2;
       if (values[2] < isolevel) cubeindex |= 4;
       if (values[3] < isolevel) cubeindex |= 8;
       if (values[4] < isolevel) cubeindex |= 16;
       if (values[5] < isolevel) cubeindex |= 32;
       if (values[6] < isolevel) cubeindex |= 64;
       if (values[7] < isolevel) cubeindex |= 128;

       /* Cube is entirely in/out of the surface */
       if (edgeTable[cubeindex] == 0)
          return triangles;

       /* Find the vertices where the surface intersects the cube */
       if (edgeTable[cubeindex] & 1)
          vertlist[0] = VertexInterp(isolevel,gridVertices[0],gridVertices[1],values[0],values[1]);
       if (edgeTable[cubeindex] & 2)
          vertlist[1] = VertexInterp(isolevel,gridVertices[1],gridVertices[2],values[1],values[2]);
       if (edgeTable[cubeindex] & 4)
          vertlist[2] = VertexInterp(isolevel,gridVertices[2],gridVertices[3],values[2],values[3]);
       if (edgeTable[cubeindex] & 8)
          vertlist[3] = VertexInterp(isolevel,gridVertices[3],gridVertices[0],values[3],values[0]);
       if (edgeTable[cubeindex] & 16)
          vertlist[4] = VertexInterp(isolevel,gridVertices[4],gridVertices[5],values[4],values[5]);
       if (edgeTable[cubeindex] & 32)
          vertlist[5] = VertexInterp(isolevel,gridVertices[5],gridVertices[6],values[5],values[6]);
       if (edgeTable[cubeindex] & 64)
          vertlist[6] = VertexInterp(isolevel,gridVertices[6],gridVertices[7],values[6],values[7]);
       if (edgeTable[cubeindex] & 128)
          vertlist[7] = VertexInterp(isolevel,gridVertices[7],gridVertices[4],values[7],values[4]);
       if (edgeTable[cubeindex] & 256)
          vertlist[8] = VertexInterp(isolevel,gridVertices[0],gridVertices[4],values[0],values[4]);
       if (edgeTable[cubeindex] & 512)
          vertlist[9] = VertexInterp(isolevel,gridVertices[1],gridVertices[5],values[1],values[5]);
       if (edgeTable[cubeindex] & 1024)
          vertlist[10] = VertexInterp(isolevel,gridVertices[2],gridVertices[6],values[2],values[6]);
       if (edgeTable[cubeindex] & 2048)
          vertlist[11] = VertexInterp(isolevel,gridVertices[3],gridVertices[7],values[3],values[7]);


       /* Create the triangle */
       for (i=0;triTable[cubeindex][i]!=-1;i+=3) {
          Triangle triangle;
          triangle.vertices[0] = (vertlist[triTable[cubeindex][i  ]]);
          triangle.vertices[1] = (vertlist[triTable[cubeindex][i+1]]);
          triangle.vertices[2] = (vertlist[triTable[cubeindex][i+2]]);
          triangles.push_back(triangle);
       }

       return triangles;
    }


/*
   Linearly interpolate the position where a surface cuts an edge between two vertices
   Adapted from http://paulbourke.net/geometry/polygonise/
*/

    EvolveMorphTrimeshExperiment::Coordinates EvolveMorphTrimeshExperiment::VertexInterp(double isolevel,Coordinates p1, Coordinates p2,double valp1,double valp2) {
        double mu;
        Coordinates p;

        if (abs(isolevel-valp1) < 0.00001)
            return(p1);
        if (abs(isolevel-valp2) < 0.00001)
            return(p2);
        if (abs(valp1-valp2) < 0.00001)
          return(p1);
        mu = (isolevel - valp1) / (valp2 - valp1);
        //cout << "iso: " << isolevel << " valp1: " << valp1 << " valp2: " << valp2 << " mu: " << mu << endl;
        p.x = p1.x + mu * (p2.x - p1.x);
        p.y = p1.y + mu * (p2.y - p1.y);
        p.z = p1.z + mu * (p2.z - p1.z);
        //cout << p.x << " " << p.y << " " << p.z << endl;
        return p;
    }

	void EvolveMorphTrimeshExperiment::getOutputImage(shared_ptr<NEAT::GeneticIndividual> individual, /*string outputNode,*/ string outputFileName)
	{

        NEAT::FastNetwork<double> network = individual->spawnFastPhenotypeStack<double>();
	    ofstream *out = new ofstream(outputFileName.c_str());


	    for(double y = -2.0; y<2.0; y+=PIXEL_SIZE) {
     		for(double x = -2.0; x<2.0; x+=PIXEL_SIZE) {
                network.reinitialize();
                network.setValue("X1",x);
                network.setValue("Y1",y);
                network.setValue("Z1",0);

                network.setValue("Bias",0.3);
                network.update();

                (*out) << network.getValue("Matter"/*outputNode*/) << " ";
	    	}
	    	(*out) << endl;
	    }
        out->close();
        delete out;
     }


#ifndef HCUBE_NOGUI
    void EvolveMorphTrimeshExperiment::createIndividualImage(wxDC &drawContext,shared_ptr<NEAT::GeneticIndividual> individual)
    {}
#endif

    #if DUMP_ALL
    static int globalCount = 0;
    #endif
    void EvolveMorphTrimeshExperiment::EvaluateIndividual(shared_ptr<NEAT::GeneticIndividual> individual, bool postHoc) {
    	entropy = 0.0;
		bool randomSelection = (Globals::getSingleton()->getParameterValue("RandomSelection",0.0) > 0.5);

        NEAT::FastNetwork<double> network = individual->spawnFastPhenotypeStack<double>();
		#if DUMP_ALL
		char debugFileName[100];
        sprintf(debugFileName, "/users/j/e/jeauerba/HyperNEAT/HyperNEAT/out/test/%05d_debug.dat",globalCount);
		ofstream *debugOut = new ofstream(debugFileName);
		#endif

        //marching cubes... find triangles on surface
        vector<Triangle> triangles;


        double gridStep = Globals::getSingleton()->getParameterValue("GridStep",0.2);//0.2;

        double xMin, yMin, zMin, xMax, yMax, zMax;
        xMax = yMax = zMax = Globals::getSingleton()->getParameterValue("MaxCoord",1.0);
        xMin = yMin = zMin = -1.0 * xMax;

		int numGridPoints = 0;

        for(double x = xMin; x< xMax; x+= gridStep) {
            for(double y = yMin; y< yMax; y+= gridStep) {
                for(double z = zMin; z< zMax; z+= gridStep) {
                    numGridPoints++;
                    //build cube
                    vector<Coordinates> gridVertices;
                    for(int i=0; i<8; i++) {
                        Coordinates p;
                        gridVertices.push_back(p);
                    }
                    gridVertices[0].x = gridVertices[3].x = gridVertices[4].x = gridVertices[7].x = x;
                    gridVertices[1].x = gridVertices[2].x = gridVertices[5].x = gridVertices[6].x = x + gridStep;
                    gridVertices[0].y = gridVertices[1].y = gridVertices[4].y = gridVertices[5].y = y;
                    gridVertices[2].y = gridVertices[3].y = gridVertices[6].y = gridVertices[7].y = y + gridStep;
                    gridVertices[0].z = gridVertices[1].z = gridVertices[2].z = gridVertices[3].z = z;
                    gridVertices[4].z = gridVertices[5].z = gridVertices[6].z = gridVertices[7].z = z + gridStep;
                    vector<double> values;
                    for(int i=0; i<8; i++) {                      
                        if(abs(gridVertices[i].x) > (xMax - gridStep/2.0) ||  abs(gridVertices[i].y) > (yMax - gridStep/2.0) || abs(gridVertices[i].z) > (zMax - gridStep/2.0)) {
                        	values.push_back(0);
                        } else {

                            network.reinitialize();
                            network.setValue("X1",gridVertices[i].x);
                            network.setValue("Y1",gridVertices[i].y);
                            network.setValue("Z1",gridVertices[i].z);

                            network.setValue("Bias",0.3);
                            network.update();
                            double matter = network.getValue("Matter");
                            values.push_back( matter );
                            //values.push_back(1);
                            #if DUMP_ALL
						    (*debugOut) << matter << "\n";
						    #endif
                        }
                    }
                    vector<Triangle>  tris = Polygonise(gridVertices, values, 0.5);

                    for(int i=0; i<tris.size(); i++) {
                        triangles.push_back(tris[i]);
                    }
                }
            }
        }

		#if DUMP_ALL
		for(int i=0; i<triangles.size(); i++) {
		(*debugOut) << "(" << triangles[i].vertices[0].x << " " << triangles[i].vertices[0].y << " " << triangles[i].vertices[0].z << ") ";
		(*debugOut) << "(" << triangles[i].vertices[1].x << " " << triangles[i].vertices[1].y << " " << triangles[i].vertices[1].z << ") ";
		(*debugOut) << "(" << triangles[i].vertices[2].x << " " << triangles[i].vertices[2].y << " " << triangles[i].vertices[2].z << ")\n";
        }
        #endif

        //now need to split up into individual objects
        set<MyObject*> objects;
        map<Coordinates, MyObject* > vertexObjectMap;

        double smallestZ = 1000;

        for(int i=0; i<triangles.size(); i++) {
            set<MyObject*> objectsIn;
            for(int j=0; j<3; j++) {
                //get smallest z coord
                if(triangles[i].vertices[j].z < smallestZ)
                    smallestZ = triangles[i].vertices[j].z;

                if(vertexObjectMap.count(triangles[i].vertices[j]) > 0) {//already seen this vertex, so note which object it is in
                    objectsIn.insert(vertexObjectMap[triangles[i].vertices[j]]);
                }
            }
            MyObject *object;
            if(objectsIn.size() == 0) { //if none of this triangles vertices are in an object create a new one
                object = new MyObject();
                objects.insert(object);
            } else { 			//otherwise loop through object to find "smallest"  this is needed to ensure there is no non-deterministic behavior based on sorting of memory addresses
                object = *(objectsIn.begin());
                for(set<MyObject* >::iterator it = objectsIn.begin(); it != objectsIn.end(); it++) {
                	if(CompareObjects((*it), object))
                		object = (*it);
                }
            }

            for(int j=0; j<3; j++) {
                object->vertices.insert(triangles[i].vertices[j]); //add all vertices to this object (is a set so don't worry about duplicates)
                vertexObjectMap[triangles[i].vertices[j]] = object;//map this vertex to that object
            }
            object->triangles.push_back(triangles[i]);		   //add this triangle to the object

            if(objectsIn.size() > 1) {
                for(set<MyObject* >::iterator it = objectsIn.begin(); it != objectsIn.end(); it++) {
                    if((*it) != object) {  //for all other objects that this vertex is in, add that object's vertices to "object"
                        for(set<Coordinates>::iterator it2 = (*it)->vertices.begin(); it2 != (*it)->vertices.end(); it2++) {
                            vertexObjectMap[*it2] = object;
                        }
                        object->vertices.insert((*it)->vertices.begin(), (*it)->vertices.end());
                        object->triangles.insert(object->triangles.end(), (*it)->triangles.begin(), (*it)->triangles.end());
                        objects.erase(*it);
                        delete *it;
                    }
                }
            }

            #if DUMP_ALL
            (*debugOut) << "*" << objectsIn.size() << " " << objects.size() << endl;
	    	#endif
        }


		#if DUMP_ALL

		for(set<MyObject*>::iterator it = objects.begin(); it!= objects.end(); it++) {
		        for(set<Coordinates>::iterator it2 = (*it)->vertices.begin(); it2 != (*it)->vertices.end(); it2++ ) {
		            (*debugOut) << "**" << it2->x << " " << it2->y << " " << it2->z << endl;
		        }

		}
		#endif


		//to be extra safe there is no non-determinism in which object is chose we first sort and then use sorted vector to find biggest
		vector<MyObject*> sortedObjects(objects.begin(), objects.end());
		stable_sort(sortedObjects.begin(), sortedObjects.end(), CompareObjects);
	
		//won't use set anymore, so can clear
		objects.clear();	
	
        //for now just get biggest object
        MyObject *biggest = NULL;
        if(sortedObjects.size() > 1) {
            for(vector<MyObject*>::iterator it = sortedObjects.begin(); it!= sortedObjects.end(); it++) {
                if(biggest == NULL || ((*it)->triangles.size() > biggest->triangles.size())) {
                    biggest = *it;
                }
            }
	
	    	//removing other objects from vector
            for(vector<MyObject*>::iterator it = sortedObjects.begin(); it!= sortedObjects.end(); ) {
                if ((*it  != biggest))
                {
                    //cout << "deleting " << *it << endl;
                    delete *it;
                    it = sortedObjects.erase(it);
                    //cout << sortedObjects.size() << endl;
                }
                else {
                    ++it;
                }
            }
            smallestZ = 1000;
            for(vector<MyObject*>::iterator it = sortedObjects.begin(); it!= sortedObjects.end(); it++) {
	            for(set<Coordinates>::iterator it2 = (*it)->vertices.begin(); it2 != (*it)->vertices.end(); it2++ ) {
        	        if(it2->z < smallestZ)
        	            smallestZ = it2->z;
        	    }
	    	}
            
        }

		vector<double> fitnesses;
		vector<int> seeds;
		int numEvals = (int) Globals::getSingleton()->getParameterValue("NumEvals",1);
		bool evalSuccess = false;
		//here there will only be 1 eval, but this code was written so could evaluate each individual in multiple simulations if desired
		for(int eval = 0; eval < numEvals; eval++) {	
			if(postHoc && eval > 0)
				break;	
			if(sortedObjects.size() > 0) {
			    ofstream *indicesOut = new ofstream(tempFileName1);
			    (*indicesOut) << sortedObjects.size() << "\n";

			    ofstream *verticesOut = new ofstream(tempFileName2);
			    (*verticesOut) << sortedObjects.size() << "\n";

			    int numVertices = 0;

			    for(vector<MyObject*>::iterator it = sortedObjects.begin(); it!= sortedObjects.end(); it++) {
			        (*indicesOut) << ((*it)->triangles.size() * 3) << "\n";
			        map<Coordinates, int> vertexMap;
			        vector<Coordinates> vertices;

			        for(int i=0; i<(*it)->triangles.size(); i++) {

			            for(int j=0; j<3; j++) {
			                int index;
				            if(vertexMap.count((*it)->triangles[i].vertices[j]) > 0) {
				                index = vertexMap[(*it)->triangles[i].vertices[j]];
				            } else {
				                index = vertices.size();
				                vertexMap[(*it)->triangles[i].vertices[j]] = index;
				                vertices.push_back((*it)->triangles[i].vertices[j]);
				            }
				            (*indicesOut) << index;
				            if( j < 2 )
				                (*indicesOut) << " ";
				        }
		            	(*indicesOut) << "\n";
		        	}

				    (*verticesOut) << vertices.size() << "\n";
				    numVertices += vertices.size();

				    //double mins[] = {100,100,100}, maxes[] = {-100,-100,-100};
				    for(int i=0; i<vertices.size(); i++) {
				        (*verticesOut) << vertices[i].x << " " << vertices[i].y << " " << (vertices[i].z - smallestZ) << "\n";
					}
		    	}

				verticesOut->close();
				indicesOut->close();
				delete verticesOut;
				delete indicesOut;
				FileMove(tempFileName1,indicesFileName);
				FileMove(tempFileName2,verticesFileName);
	
				//other experiments looked at having different random seeds sent to simulator for generating different environments, but here environments are not random
		    	int r = 1;

			    ofstream *paramsOut = new ofstream(tempFileName1);
			    for(int i =0; i<individual->getParamsCount(); i++) {
			        GeneticParamGene *param = individual->getParam(i);
			        (*paramsOut) << param->getName() << " " << param->getValue() << "\n";
			    }
			    (*paramsOut) << "ActualRandomSeed" << " " << r << "\n";
			    seeds.push_back(r);
			    //cout << "adding params\n";
			    			    
			    paramsOut->close();
			    delete paramsOut;


				#if DUMP_ALL
				char tmp[100];
				sprintf(tmp, "/users/j/e/jeauerba/HyperNEAT/HyperNEAT/out/test/%05d_indices.dat",globalCount);
			    FileCopy(indicesFileName, tmp);
				sprintf(tmp, "/users/j/e/jeauerba/HyperNEAT/HyperNEAT/out/test/%05d_vertices.dat",globalCount);
			    FileCopy(verticesFileName, tmp);
				sprintf(tmp, "/users/j/e/jeauerba/HyperNEAT/HyperNEAT/out/test/%05d_params.dat",globalCount);
			    FileCopy(tempFileName1, tmp);
				#endif

				FileMove(tempFileName1,paramsFileName);
				FileTouch(readyFileName); //once all files have been created including the readyFile then simulator will start
			
			    GetResults();


		    	if(results.size() > 0) {
		    		evalSuccess=true;
				    double start[] = {results[0].second.x, results[0].second.y, results[0].second.z};
				    double end[] = {results[results.size() - 1].first.x, results[results.size() - 1].first.y, results[results.size() - 1].first.z};

				    double dist = (end[0] - start[0]);
				    if(postHoc) {
				        printf("dist: %5.5f\n", dist);
				    }
	
		        	if( randomSelection ) {	
				        fitnesses.push_back(2.0 + Globals::getSingleton()->getRandom().getRandomDouble(1.0, 10.0));
				    } else {		        
				        fitnesses.push_back(10000.0 + dist);
		        	}
			    } else {
			    	fitnesses.push_back(2.0); //give 2 if went to simulator but simulation failed
		    	}

			} else {
			  	fitnesses.push_back(1.0); //give 1 if no valid trimesh created
			  	seeds.push_back(-1);
			}
		}
		//if(evalSuccess)
		//	printf("entropy: %5.5f\n", entropy);
		//else
		//	printf("entropy: NO\n");
	
		double fitness = fitnesses[0];	
		int actualRandomSeed = seeds[0];
	
		for(int eval=1; eval<fitnesses.size(); eval++) {
			if(fitnesses[eval] < fitness) { //use min fitness across evaluations if multiple evals
				fitness = fitnesses[eval];
				actualRandomSeed = seeds[eval];			
			}
		}	
		individual->setUserData(shared_ptr<GeneticIndividual::StringableImpl>(new GeneticIndividual::StringableImpl(JGTL::toString(entropy))));	
	
		if(Globals::getSingleton()->getParameterValue("MultiObjective", 0.0) > 0.5) {
			individual->setFitness(fitness / (1.0 + entropy));	//uni-objective combined fitness, never actually used in multi-objective case
			//add second fitness term (to minimize entropy)... this is used in multi-objective case
			if(fitness > 2.0)	
				fitnesses.push_back(1.0 / (1.0 + entropy));	
			else
				fitnesses.push_back(0);
		} else {
			individual->setFitness(fitness);
		}
		individual->setFitnesses(fitnesses);	

        for(vector<MyObject*>::iterator it = sortedObjects.begin(); it!= sortedObjects.end(); it++) {
            delete *it;
        }
        vertexObjectMap.clear();
        sortedObjects.clear();
        triangles.clear();


		#if DUMP_ALL
		stringstream ss;
		ss << globalCount;
		TiXmlDocument *doc = new TiXmlDocument( string("/users/j/e/jeauerba/HyperNEAT/HyperNEAT/out/test/") + ss.str() + ".xml" );
		TiXmlElement *individualElement = new TiXmlElement("Individual");
		    individual->dump(individualElement);
		doc->LinkEndChild(individualElement);
		doc->SaveFile();
		delete doc;
		globalCount++;
		debugOut->close();
		delete debugOut;

		#endif
    }


    void EvolveMorphTrimeshExperiment::GetResults() {
            while( !( FileExists(inFileName) && FileExists(readyFileName) ) ) {
                boost::system_time time = boost::get_system_time();
                time += boost::posix_time::milliseconds(5);
                boost::thread::sleep(time);
            }

            numGroundContacts = 0;

            results.clear();
            ifstream *in = new ifstream(inFileName);
            int T;
            (*in) >> T;
            if(T > 0) {
            	(*in)  >> entropy;
                for(int i=0; i<T; i++) {
                    Coordinates smallestX;
                    (*in) >> smallestX.x;
                    (*in) >> smallestX.y;
                    (*in) >> smallestX.z;
                    Coordinates largestX;
                    (*in) >> largestX.x;
                    (*in) >> largestX.y;
                    (*in) >> largestX.z;                    
                    results.push_back(make_pair(smallestX,largestX)); 
                }
            }

            in->close();
            delete in;

            FileDelete(inFileName);
            FileDelete(readyFileName);
    }

    double EvolveMorphTrimeshExperiment::Variance(vector<double> vals) {
        double mean = Mean(vals);
        double sumOfSquaredDeviations = 0;
        for(int i = 0; i<vals.size(); i++) {
            sumOfSquaredDeviations += pow(mean - vals[i], 2.0);
        }
        return sumOfSquaredDeviations / double(vals.size());

    }

    double EvolveMorphTrimeshExperiment::Mean(vector<double> vals) {
        double sum = 0;
        for(int i = 0; i < vals.size(); i++) {
            sum += vals[i];
        }
        return (sum/double(vals.size()));

    }

    Experiment* EvolveMorphTrimeshExperiment::clone()
    {
        EvolveMorphTrimeshExperiment* experiment = new EvolveMorphTrimeshExperiment(*this);

        return experiment;
    }
    
    void  EvolveMorphTrimeshExperiment::FileCopy(char *src, char *dest) {
		char command[200];
		sprintf(command,"cp %s %s", src, dest);
		std::system(command);
	}

	void  EvolveMorphTrimeshExperiment::FileMove(char *src, char *dest) {
	    char command[200];
	    sprintf(command,"mv %s %s", src, dest);
	    std::system(command);
	}

	void  EvolveMorphTrimeshExperiment::FileDelete(char *fileName) {
		char command[200];
		sprintf(command,"rm %s",fileName);
		std::system(command);
	}

	int EvolveMorphTrimeshExperiment::FileExists(char *fileName) {
		int exists;
		ifstream *inFile = new ifstream(fileName);
		if ( inFile->good() )
		    exists = true;
		else
		    exists = false;

		inFile->close();
		delete inFile;
		inFile = NULL;

		return( exists );
	}
	
	void  EvolveMorphTrimeshExperiment::FileTouch(char *fileName) {
		char command[200];
		sprintf(command,"touch %s",fileName);
		std::system(command);
	}



}

