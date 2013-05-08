#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "fstream"
#include "GlobalFunctions.h"

#include <GL/glut.h>

#include <sstream>

#ifdef _MSC_VER
#pragma warning(disable:4244 4305)  // for VC++, no precision loss complaints
#endif

// select correct drawing functions

#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#define dsDrawLine dsDrawLineD
#define dsDrawTriangle dsDrawTriangleD
#endif

// some constants
#define NUM 200			// max number of objects
#define DENSITY (0.1)		// density of all objects

#define PI (3.14159)

#define CAP_ACCEL 1
#define REFLECT_AND_COPY 1
#define STOP_EARLY 1
#define DISABLE_GEOMS 1

// dynamics and collision objects

using namespace std;

struct MyObject {
  dBodyID body;			// the body
  dGeomID geom;			// geomety representing this body

  int VertexCount;
  int IndexCount;
  dVector3* Vertices;
  dTriIndex* Indices;

};

struct MyJoint {
    dJointID joint;
    double period;
    double phaseShift;
};

static int num=0;		// number of objects in simulation
static int numJoints = 0;
static dWorldID world;
static dSpaceID space;
static MyObject obj[NUM];

static MyJoint joint[10];

static dJointGroupID contactgroup;

static dGeomID floorPlane;

static dGeomID boxes[1000];
static int numBoxes = 0;
#if DISABLE_GEOMS
static int enabledStart =0;
static int enabledEnd =0;
static double enabledWindow = 4.0;
#endif


static int t;

typedef dReal dVector3R[3];

static char tempFileName[200];
static char temp2FileName[200];
static char indicesFileName[200];
static char verticesFileName[200];
static char resultsFileName[200];
static char paramsFileName[200];
static char readyFileName[200];

static bool evaluating = false;
static bool capture = false;
static int frame=0;
static int frameRate = 0;
static bool rendering = true;
static int numTimeSteps = (5 * 2500);//5000;//2000;

static bool runOnce = false;
static int seed = -1;
static int thread = -1;

static bool evaluationCanceled = false;

static vector< vector<dReal> > positions;
static vector<vector<dReal> > linVels, angVels;

static string picture = "";
static ofstream *contactsOut = NULL;


static bool sideView = false;

static double boxSpacing = 0.2;
static double boxWidth = 0.2;
static double boxHeight = 0;//0.1;
static double boxAngle = 0;//PI/10.0;
static double gravityAngle = 0; //0 = -z, 90 = -x


static double entropy;
static string baseDir = "../HyperNEAT/out/tmp";


// this is called by dSpaceCollide when two objects in space are
// potentially colliding.
static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
  int i;

  // exit without doing anything if the two bodies are connected by a joint
  dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);

  if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact))
  	return;

  //if both are parts of ground then get out
  if (b1 == 0 && b2 == 0)
  	return;

  int N = 128;
  dContact contact[N];

  double mu;// = 0;
  if(dGeomGetClass(o1) == dBoxClass || dGeomGetClass(o2) == dBoxClass) {
  	//if collision with box then 0 friction
  	mu = 0;
  } else {
  	//if self collision, or collision with ground plane then infinite friction
  	mu = dInfinity;
  }



  if (int numc = dCollide (o1,o2,N,&contact[0].geom, sizeof(dContact))) {
    //cout << numc << " contacts between " << dGeomGetClass(o1) << " and " << dGeomGetClass(o2) << " (" << o1 << " and " << o2 << ")\n";
    for (i=0; i<numc; i++) {
            contact[i].surface.slip1 = 0.01;
            contact[i].surface.slip2 = 0.01;
            contact[i].surface.mode = dContactSoftERP |
                        dContactSoftCFM |
                        dContactApprox1 |
                        dContactSlip1 | dContactSlip2;

            contact[i].surface.mu = mu;
            contact[i].surface.soft_erp = 0.96;
            contact[i].surface.soft_cfm = 0.01;

	    if(dGeomGetClass(o1) == dBoxClass || dGeomGetClass(o2) == dBoxClass) {
	    	//cout << "(" << t << ") box collision at height " << contact[i].geom.pos[2] << ", boxHeight= " << boxHeight << ", mu= " << contact[i].surface.mu << endl;
	    	if( (contact[i].geom.pos[2] + 0.05) >= boxHeight) {
	    		dBodyID b;
	    		if(dGeomGetClass(o1) == dBoxClass)
	    			b = b2;
	    		else
	    			b = b1;
	    	}
	    } else {
	    	//cout << "(" << t << ") ground collision at height " << contact[i].geom.pos[2] << ", boxHeight= " << boxHeight << ", mu= " << contact[i].surface.mu << endl;	    
	    }

	    if(contactsOut != NULL) {
	    	cout << "saving contact info\n";   	    
			(*contactsOut) << t << " " << dGeomGetClass(contact[i].geom.g1) << " " << dGeomGetClass(contact[i].geom.g2) << " ";
			(*contactsOut) << contact[i].geom.pos[0] << " " << contact[i].geom.pos[1] << " " << contact[i].geom.pos[2] << " ";
			(*contactsOut) << contact[i].geom.normal[0] << " " << contact[i].geom.normal[1] << " " << contact[i].geom.normal[2] << " ";
			(*contactsOut) << contact[i].geom.depth;
			(*contactsOut) << endl;

	    }

		dJointID c = dJointCreateContact (world,contactgroup,&contact[i]);
		dJointAttach (c,b1,b2);
    }
  }
}

// start simulation - set viewpoint

static void start()
{
  dAllocateODEDataForThread(dAllocateMaskAll);
  if(sideView) {
    //side view
    static float xyz[3] = {0/*3.6163*/,-6.8299,2.7000};
    static float hpr[3] = {100.5000,-26.0000,0.0000};
    dsSetViewpoint (xyz,hpr);
  } else {
    //rear view
    static float xyz[3] = {0.8276,-3.7560,1.9800};//{-6.4232,-0.6764,5.4300};
    static float hpr[3] = {90.0000,-38.0000,0.0000};//6.5000,-23.0000,0.0000};
    dsSetViewpoint (xyz,hpr);
  }
}


// called when a key pressed
static void command (int cmd)
{
}

static void CaptureFrame(int num) {


	int width = 352*2;
	int height = 288*2;
	char s[200];

	if(num < 0) {
	   sprintf (s,"%s/frame/%s.ppm",baseDir.c_str(), picture.c_str());
      	   printf("taking picture\n");
	} else {
		printf("capturing frame %04d\n",num);
		sprintf (s,"%s/frame/%04d.ppm",baseDir.c_str(),num);
	}
	FILE *f = fopen (s,"wb");
	fprintf (f,"P6\n%d %d\n255\n",width,height);

	void *buf = malloc( width * height * 3 );
	glReadPixels( 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buf );

	for (int y=(height - 1); y>=0; y--) {
		for (int x=0; x<width; x++) {
			unsigned char *pixel = ((unsigned char *)buf)+((y*width+ x)*3);
			unsigned char b[3];
			b[0] = *pixel;
			b[1] = *(pixel+1);
			b[2] = *(pixel+2);
			fwrite(b,3,1,f);
		}
	}
	free(buf);
	fclose(f);

}

bool isStopped() {
    for(int i=0; i<num; i++) {
        const dReal *linVel = dBodyGetLinearVel(obj[i].body);
        double linVelMagnitude = GlobalFunctions::VectorLength(linVel, 3);

        const dReal *angVel = dBodyGetAngularVel(obj[i].body);
        double angVelMagnitude = GlobalFunctions::VectorLength(angVel, 3);

        if(linVelMagnitude > (10.0 * EPSILON) || angVelMagnitude > (10.0 * EPSILON))
            return false;

    }
    printf("stopped\n");
    return true;

}

static void drawObject(MyObject *object) {
	if(object->geom == NULL)
		return;
	if (dGeomGetClass(object->geom) == dTriMeshClass) {
		dsSetColor (1,1,0);
		const dReal* Pos = dGeomGetPosition(object->geom);
		const dReal* Rot = dGeomGetRotation(object->geom);


		for (int ii = 0; ii < object->IndexCount / 3; ii++) {
		    const dReal v[9] = {
		      object->Vertices[object->Indices[ii * 3 + 0]][0],
		      object->Vertices[object->Indices[ii * 3 + 0]][1],
		      object->Vertices[object->Indices[ii * 3 + 0]][2],
		      object->Vertices[object->Indices[ii * 3 + 1]][0],
		      object->Vertices[object->Indices[ii * 3 + 1]][1],
		      object->Vertices[object->Indices[ii * 3 + 1]][2],
		      object->Vertices[object->Indices[ii * 3 + 2]][0],
		      object->Vertices[object->Indices[ii * 3 + 2]][1],
		      object->Vertices[object->Indices[ii * 3 + 2]][2]
		    };
		    dsDrawTriangle(Pos, Rot, &v[0], &v[3], &v[6], 1);
		}
	} else if(dGeomGetClass(object->geom) == dSphereClass) {
	    dsSetColor (1,0,0);
	    dsDrawSphere (dGeomGetPosition(object->geom),dGeomGetRotation(object->geom),dGeomSphereGetRadius(object->geom));
	} else if (dGeomGetClass(object->geom) == dCapsuleClass) {
	    dReal radius,length;
	    dGeomCapsuleGetParams (object->geom,&radius,&length);
	    dsDrawCapsule (dGeomGetPosition(object->geom),dGeomGetRotation(object->geom),length,radius);
	} else if (dGeomGetClass(object->geom) == dBoxClass) {
	    dVector3 sides;
	    dGeomBoxGetLengths (object->geom,sides);
	    dsDrawBox (dGeomGetPosition(object->geom),dGeomGetRotation(object->geom),sides);
	}


}

static void drawObjects() {
	for (int i=0; i<num; i++) {
	  	drawObject(&obj[i]);
	}

	for (int i=0; i<numBoxes; i++) {
		dsSetColor (0,0.5,0.5);
		dVector3 sides;
		dGeomBoxGetLengths (boxes[i],sides);
		dsDrawBox (dGeomGetPosition(boxes[i]),dGeomGetRotation(boxes[i]),sides);
	}
}

static void actuate(dJointID joint, dReal position) {

	dJointSetHingeParam(joint,dParamFMax,0.5);
	
	dReal desiredAngle = position;
	dReal actualAngle =  dJointGetHingeAngle(joint);
	//cout << "actual: " << actualAngle << ", desired: " << desiredAngle << endl;
	dReal actualRate = dJointGetHingeAngleRate(joint);

	dReal ks = 3;// * energy;//5;//motorSpeed;
	dReal kd = 0.0;
	dReal error = ks*(desiredAngle - actualAngle) - kd*actualRate;

	dJointSetHingeParam(joint,dParamVel,error);
}

void Tick(int pause, int shouldDraw) {
  if( (evaluating) && (t < numTimeSteps)
  	#if STOP_EARLY
  		&& (t<25 || !isStopped())
  	#endif
  	&& (!evaluationCanceled)) {

  	if(shouldDraw) {
		  drawObjects();
		  if(picture.length() > 0) {
		  	CaptureFrame(-1);
		  	exit(0);
		  }

	}
	const dReal* Pos = dGeomGetPosition(obj[2].geom);
	
	dReal smallestXCoords[] = {10000, 0, 0};
	dReal largestXCoords[] = {-10000, 0, 0};	
	for(int i=0; i<2; i++) {
		for(int j=0; j<obj[i].VertexCount; j++) {
			//cout << obj[i].Vertices[j][0] << " " << obj[i].Vertices[j][1] << " " << obj[i].Vertices[j][2] << "\n";
			dVector3 pos;
			dBodyGetRelPointPos(obj[i].body, obj[i].Vertices[j][0], obj[i].Vertices[j][1], obj[i].Vertices[j][2], pos);
			//cout << pos[0] << " " << pos[1] << " " << pos[2] << "\n";
			
			if(pos[0] < smallestXCoords[0]) {
				smallestXCoords[0] = pos[0];
				smallestXCoords[1] = pos[1];
				smallestXCoords[2] = pos[2];							
			}
			if(pos[0] > largestXCoords[0]) {
				largestXCoords[0] = pos[0];
				largestXCoords[1] = pos[1];
				largestXCoords[2] = pos[2];										
			}
		}
		
	
	}

	//this code allows for printing text with OpenGL, but requires a modification to DrawStuff, so commenting out
	/*
	if(shouldDraw) {
		dsSetColor (0,0,1);
            	dMatrix3 R;
            	dRSetIdentity(R);
        	//dsDrawSphere (smallestXCoords,R,0.05);
        	//dsDrawSphere (largestXCoords,R,0.05);        	
	
	
	
		dsSetOrthographicProjection(1400, 900); 
		const int kWidth = 20;
		const int kHeight = 30;
		string s;
		stringstream out;
  		int xc = kWidth;
		int yc = 0;

		// dummy print to get over color problem <--------- this is because color was somehow not adapted in the first print
		dsRenderBitmapString(0, 0, "");
  
		out << t;
		//out << "Energy: " << energy << "; Capsule Position: (" << Pos[0] << "," << Pos[1] << "," << Pos[2] << "); Trailing Vertex: (" << smallestXCoords[0] << "," << smallestXCoords[1] << "," << smallestXCoords[2] << ")";
		//out << "Capsule Position: (" << Pos[0] << "," << Pos[1] << "," << Pos[2] << ")\n";//; Trailing Vertex: (" << smallestXCoords[0] << "," << smallestXCoords[1] << "," << smallestXCoords[2] << ")";
		s = out.str();
		out.str("");
		yc += kHeight;
		dsRenderBitmapString(xc, yc, (char *)s.c_str()); 
		dsResetPerspectiveProjection();

	}
	*/

	if (!pause) {
		  vector<dReal> position;
 		  for(int i=0; i<3; i++) {
	 		  position.push_back(smallestXCoords[i]);
 		  }	
		  for(int i=0; i<3; i++) {
	 		  position.push_back(largestXCoords[i]);
 		  } 		  
		  positions.push_back(position);
		  if(contactsOut != NULL) {
		  	(*contactsOut) << "Position: " << t << " " << smallestXCoords[0] << " " << smallestXCoords[1] << " " << smallestXCoords[2] << " ";
		  	(*contactsOut) << largestXCoords[0] << " " << largestXCoords[1] << " " << largestXCoords[2] << "\n";
		  }

          #if DISABLE_GEOMS
          //rolling window of enabled boxes
		  if(numBoxes > 0) {
			  while( (fabs(Pos[0] - dGeomGetPosition(boxes[enabledStart])[0]) < enabledWindow ) && enabledStart > 0 ) {
				  //cout << "shifting window backwards " << enabledStart << endl;
		          enabledStart--;
		          dGeomEnable(boxes[enabledStart]);
		          dGeomDisable(boxes[enabledEnd]);
		          enabledEnd--;
			  }

			  while( (fabs(Pos[0] - dGeomGetPosition(boxes[enabledEnd])[0]) < enabledWindow) && enabledEnd < (numBoxes - 1)) {
				  //cout << "shifting window forwards " << enabledEnd << endl;
		          enabledEnd++;
		          dGeomEnable(boxes[enabledEnd]);
		          dGeomDisable(boxes[enabledStart]);
		          enabledStart++;
			  }
		  }
		  #endif

	  	  for(int i=0; i<numJoints; i++) {
	  	      actuate(joint[i].joint, dJointGetHingeParam(joint[i].joint, dParamHiStop) * sin( (2.0*PI/joint[i].period) * (t - joint[i].period * joint[i].phaseShift )) );
	  	  }
	  	  
  	  
	  	  dSpaceCollide (space,0,&nearCallback);

		  //cout << "stepping...\n";
		  dWorldStep (world,0.001);//need small step size for trimeshes to work well
		  //cout << "done stepping\n";

		  if(capture && (t % frameRate == 0)) {
		        CaptureFrame(frame);
		        frame++;
		  }

		  dJointGroupEmpty (contactgroup);

#if CAP_ACCEL
		  double maxLinAccel = 0;
		  double maxAngAccel = 0;

		  for(int i=0; i<num; i++) {
			const dReal *angVel, *linVel;
			dReal previousAngVel[3], previousLinVel[3];
			angVel = dBodyGetAngularVel(obj[i].body);
			linVel = dBodyGetLinearVel(obj[i].body);
			//cout << angVel[0] << " " << angVel[1] <<  " " << angVel[2] << endl;
			//cout << linVel[0] << " " << linVel[1] <<  " " << linVel[2] << endl;
			if(t > 0) {
				for(int j=0; j<3; j++) {
                	previousAngVel[j] = angVels[i][j];
                	previousLinVel[j] = linVels[i][j];
            	}

            	double angAccel = GlobalFunctions::EuclideanDistance(angVel, previousAngVel);
            	double linAccel = GlobalFunctions::EuclideanDistance(linVel, previousLinVel);

				if(angAccel > maxAngAccel)
					maxAngAccel = angAccel;
				if(linAccel > maxLinAccel)
					maxLinAccel = linAccel;

				vector<dReal> tmp, tmp2;
				for(int j=0; j<3; j++) {
					tmp.push_back(angVel[j]);
					//angVelSums[i][j] += angVel[j];
					tmp2.push_back(linVel[j]);
				}
		            angVels[i] = tmp;
		            linVels[i] = tmp2;
		        } else {
		            vector<dReal> tmp, tmp2;
		            for(int j=0; j<3; j++) {
		                tmp.push_back(angVel[j]);
		                tmp2.push_back(linVel[j]);
		            }
		            angVels.push_back(tmp);
		            //angVelSums.push_back(tmp);
		            linVels.push_back(tmp2);
                	}
            	}
            	//printf("max: angular accel = %5.5f, linear accel = %5.5f\n", maxAngAccel, maxLinAccel);

			if(maxAngAccel > 10.0 || maxLinAccel > 10.0) {
				evaluationCanceled = true;
				printf("EVAL_STATS: evaluation canceled: max accel exceeded at timestep %d\n", t);
			}
#endif
		t++;
	}
    } else if(evaluating) {
        evaluating = false;
        if( (t>=25) && isStopped() ) {
            printf("EVAL_STATS: early termination: body stopped at timestep %d\n", t);
        }
        if( t >= numTimeSteps ) {
            printf("EVAL_STATS: evaluation completed after %d time steps\n", numTimeSteps);
        }
    }
}

static void simLoop (int pause) {
    if (evaluating)
		Tick(pause,true);
    else
		dsStop();
}


static void parseParams(int argc, char **argv) {
    for(int currParam=0;currParam<argc;currParam++) {
        if ( strcmp(argv[currParam],"-capture") == 0 ) {
            capture = true;
            frameRate = atoi(argv[currParam+1]);
        }

        if ( strcmp(argv[currParam],"-once") == 0 ) {
            runOnce = true;
        }

        if ( strcmp(argv[currParam],"-continue") == 0 ) {
		   char s[200];

		   sprintf (s,"%s/frame/%04d.ppm",baseDir.c_str(),frame);
		   while(GlobalFunctions::FileExists(s)) {
		   	   frame++;
			   sprintf (s,"%s/frame/%04d.ppm",baseDir.c_str(),frame);
		   }
        }

        if ( strcmp(argv[currParam],"-null") == 0 ) {
            rendering = false;
        }

        if ( strcmp(argv[currParam],"-r") == 0) {
            seed = atoi(argv[currParam+1]);
        }

        if ( strcmp(argv[currParam],"-t") == 0) {
            thread = atoi(argv[currParam+1]);
        }

        if ( strcmp(argv[currParam],"-picture") == 0) {
            picture = string(argv[currParam+1]);
        }

        if ( strcmp(argv[currParam],"-contactsOut") == 0) {
		    contactsOut = new ofstream(argv[currParam+1]);
        }

        if ( strcmp(argv[currParam],"-sideView") == 0) {
		    sideView = true;
        }

        if ( strcmp(argv[currParam],"-boxSpacing") == 0) {
		    boxSpacing = atof(argv[currParam+1]);
        }

        if ( strcmp(argv[currParam],"-boxWidth") == 0) {
		    boxWidth = atof(argv[currParam+1]);
        }

        if ( strcmp(argv[currParam],"-boxHeight") == 0) {
		    boxHeight = atof(argv[currParam+1]);
        }

        if ( strcmp(argv[currParam],"-boxAngle") == 0) {
		    boxAngle = atof(argv[currParam+1]);
        }

        if ( strcmp(argv[currParam],"-gravityAngle") == 0) {
		    gravityAngle = atof(argv[currParam+1]);
        }
        
        if ( strcmp(argv[currParam],"-baseDir") == 0) {
            baseDir = string(argv[currParam+1]);
        }
    }
}

static void setupODE() {
  // create world

  dInitODE2(0);
  world = dWorldCreate();
  space = dHashSpaceCreate (0);
  contactgroup = dJointGroupCreate (0);
  double x = -9.8 * sin(gravityAngle * PI/180.0);
  double z = -9.8 * cos(gravityAngle * PI/180.0);

  cout << "gravity: " << x << ",0," << z << endl;
  dWorldSetGravity(world,x,0,z);
  dWorldSetCFM (world,0.01);

  dWorldSetAutoDisableAverageSamplesCount( world, 10 );
  dWorldSetContactMaxCorrectingVel (world,0.1);
  dWorldSetContactSurfaceLayer (world,0.001);

  dWorldSetDamping (world, 0.005, 0.005);
  floorPlane =  dCreatePlane (space,0,0,1,0);

}


static void destroyODE() {
  dJointGroupDestroy (contactgroup);
  dSpaceDestroy (space);
  dWorldDestroy (world);
  dCloseODE();
}


int main (int argc, char **argv)
{
  parseParams(argc, argv);
  if(seed >= 0) {
  	if(thread < 0) {
		sprintf(tempFileName,"%s/Files%d/temp.dat", baseDir.c_str(), seed);
		sprintf(temp2FileName,"%s/Files%d/temp2.dat", baseDir.c_str(), seed);		
		sprintf(verticesFileName,"%s/Files%d/Vertices.dat", baseDir.c_str(), seed);
		sprintf(indicesFileName,"%s/Files%d/Indices.dat", baseDir.c_str(), seed);
		sprintf(resultsFileName,"%s/Files%d/Results.dat", baseDir.c_str(), seed);
		sprintf(paramsFileName,"%s/Files%d/Params.dat", baseDir.c_str(), seed);
		sprintf(readyFileName,"%s/Files%d/Ready.dat", baseDir.c_str(), seed);
	  } else {
		sprintf(tempFileName,"%s/Files%d_%d/temp.dat", baseDir.c_str(), seed, thread);
		sprintf(temp2FileName,"%s/Files%d_%d/temp2.dat", baseDir.c_str(), seed, thread);		
		sprintf(verticesFileName,"%s/Files%d_%d/Vertices.dat", baseDir.c_str(), seed, thread);
		sprintf(indicesFileName,"%s/Files%d_%d/Indices.dat", baseDir.c_str(), seed, thread);
		sprintf(resultsFileName,"%s/Files%d_%d/Results.dat", baseDir.c_str(), seed, thread);
		sprintf(paramsFileName,"%s/Files%d_%d/Params.dat", baseDir.c_str(), seed, thread);
		sprintf(readyFileName,"%s/Files%d_%d/Ready.dat", baseDir.c_str(), seed, thread);		
  	}
  }
  srand(seed);

  // setup pointers to drawstuff callback functions
  dsFunctions fn;
  if(rendering) {
      fn.version = DS_VERSION;
      fn.start = &start;
      fn.step = &simLoop;
      fn.command = &command;
      fn.stop = 0;
      fn.path_to_textures = (char *) "textures";
  }

  vector<char*> files;
  files.push_back(verticesFileName);
  files.push_back(indicesFileName);
  files.push_back(paramsFileName);
  files.push_back(readyFileName);

  while(true) {

	  memset (obj,0,sizeof(obj));

	  setupODE();
	  bool allFilesExist;
	  do {
      	allFilesExist = true;
      	for(int i=0; i<files.size() && allFilesExist; i++) {
            	allFilesExist = (allFilesExist && GlobalFunctions::FileExists(files[i]) );
        	}
			usleep(5000); //5000 micro seconds = 5 ms
      } while(!allFilesExist);
          
      GlobalFunctions::FileDelete(readyFileName);

      cout << "reading files\n";

	  ifstream *verticesIn = new ifstream(verticesFileName);
	  ifstream *indicesIn = new ifstream(indicesFileName);
	  (*verticesIn) >> num;
	  int tmp;
	  (*indicesIn) >> tmp;
	  if(tmp != num) {
	  	cout << "ERROR!: num objects differs between input files\n\n";
	  	exit(-1);
	  }
	  cout << num << endl;
	  for(int i = 0; i < num; i++) {

		obj[i].body = dBodyCreate (world);
		dBodySetPosition (obj[i].body, 0,0,0);
		dBodySetData (obj[i].body,(void*)(size_t)i);

	  	(*verticesIn) >> obj[i].VertexCount;
	  	obj[i].Vertices = new dVector3[obj[i].VertexCount];
		for(int j=0; j<obj[i].VertexCount; j++) {
			for(int k=0; k<3; k++) {
				(*verticesIn) >> obj[i].Vertices[j][k];
			}
		}

		(*indicesIn) >> obj[i].IndexCount;
		obj[i].Indices = new dTriIndex[obj[i].IndexCount];
		for(int j=0; j< obj[i].IndexCount; j++) {
			(*indicesIn) >> obj[i].Indices[j];
		}
		dTriMeshDataID new_tmdata = dGeomTriMeshDataCreate();
		dGeomTriMeshDataBuildSimple(new_tmdata, (dReal*) obj[i].Vertices, obj[i].VertexCount, obj[i].Indices, obj[i].IndexCount);
		obj[i].geom = dCreateTriMesh(space, new_tmdata, 0, 0, 0);

		// remember the mesh's dTriMeshDataID on its userdata for convenience.
		dGeomSetData(obj[i].geom, new_tmdata);


		dMass m;
		dMassSetTrimesh( &m, DENSITY, obj[i].geom );
		printf("mass of %f at %f %f %f\n", m.mass, m.c[0], m.c[1], m.c[2]);
		if(m.mass < 0.0001)
			exit(-1);
		dGeomSetPosition(obj[i].geom, -m.c[0], -m.c[1], -m.c[2]);
		dMassTranslate(&m, -m.c[0], -m.c[1], -m.c[2]);

		dGeomSetBody (obj[i].geom,obj[i].body);
		dBodySetMass (obj[i].body,&m);
		
		
		double angleExcesses[obj[i].VertexCount];  //curvatures at each vertex
		double totalDefect = 0;
		for(int v = 0; v < obj[i].VertexCount; v++) {
			//for each vertex we want to find all triangles it is part of, 
			//compute angle at that vertex and area of triangles			
			double area = 0;
			double angleSum = 0;
			for(int j=0; j< obj[i].IndexCount; j+=3) {
				dReal v1[3], v2[3];
				int a,b,c;
				if( obj[i].Indices[j] == v ) {
					a = j;
					b = j+1;
					c = j+2;
				} else if( obj[i].Indices[j+1] == v) {
					a = j+1;
					b = j;
					c = j+2;			
				} else if( obj[i].Indices[j+2] == v) {
					a = j+2;
					b = j;
					c = j+1;
				} else {
					continue;
				}
				double dotProduct = 0;
				for(int k=0; k<3; k++) {
					v1[k] = obj[i].Vertices[obj[i].Indices[b]][k] - obj[i].Vertices[obj[i].Indices[a]][k];
					v2[k] = obj[i].Vertices[obj[i].Indices[c]][k] - obj[i].Vertices[obj[i].Indices[a]][k];
					dotProduct += v1[k]*v2[k];
				}
				
				double angle = acos(  dotProduct / (GlobalFunctions::VectorLength(v1,3) * GlobalFunctions::VectorLength(v2,3) ) );
				
				angleSum += angle;
				area += (0.5 * GlobalFunctions::VectorLength(GlobalFunctions::CrossProduct(v1,v2),3));				
			}
			
			angleExcesses[v] = (2*PI) - angleSum;
  	  	}
		
		ofstream *out = new ofstream(tempFileName);
		(*out) << "angleExcesses: ";
		for(int v = 0; v < obj[i].VertexCount; v++) {
			if(v>0)
				(*out) << ",";
			(*out) << angleExcesses[v];
		}
		(*out) << endl;
		out->close();
		
		char command[200];
		sprintf(command,"python entropy.py %s %s", tempFileName, temp2FileName);
		system(command);
		
		ifstream *entropyIn = new ifstream(temp2FileName);
		(*entropyIn) >> entropy;
		cout << "entropy: " << entropy << "\n";		
		
	  }

	  verticesIn->close();
	  delete verticesIn;
	  indicesIn->close();
	  delete indicesIn;

	  ifstream *paramsIn = new ifstream(paramsFileName);
	  map<string, double> params;
	  while(paramsIn->good()) {
	      string key;
	      (*paramsIn) >> key;
	      if(!paramsIn->good())
            break;
	      double value;
	      (*paramsIn) >> value;
	      params[key] = value;
	      //cout << key << " " << params[key] << endl;
	  }
	  paramsIn->close();
	  delete paramsIn;
	  
	  if(params.count("ActualRandomSeed") > 0) {
	  	cout << "Actual Random Seed: " << ((int) params["ActualRandomSeed"]) << endl;
	  	srand((int) params["ActualRandomSeed"]);
	  }
	  
	  

	  if(params.count("BoxHeight") > 0)
        boxHeight = params["BoxHeight"];
      if(params.count("BoxWidth") > 0)
        boxWidth = params["BoxWidth"];
      if(params.count("BoxSpacing") > 0)
        boxSpacing = params["BoxSpacing"];
      if(params.count("BoxAngle") > 0)
        boxAngle = params["BoxAngle"];

      if(boxHeight > 0) {  //set up blocks of ice (in boxes array)      	
      
         for(int i=0; i<numBoxes; i++) {
             dGeomDestroy(boxes[i]);
         }
         #if DISABLE_GEOMS
         enabledStart = -1;
         enabledEnd = -1;
         enabledWindow = 10*(boxWidth+boxSpacing);
         #endif

         numBoxes = 0;

		 int loopMax = (int) ceil(50/(boxWidth+boxSpacing));

         for(int i=0; i<loopMax; i++) {         

            boxes[i] = dCreateBox(space, boxWidth, 50, 2*boxHeight);
            dGeomSetPosition(boxes[i], i*(boxWidth + boxSpacing) - 25, 0, 0);
            dGeomSetBody(boxes[i],0);
            dMatrix3 R;
            dRFromAxisAndAngle (R,0,1,0,boxAngle);
            dGeomSetRotation(boxes[i], R);


            #if DISABLE_GEOMS
            if(fabs(i*(boxWidth + boxSpacing) - 25) >= enabledWindow) {
                //cout << "disabling " << i << ", x-pos: " << i*(boxWidth + boxSpacing) - 25 << endl;
                dGeomDisable(boxes[i]);
            } else {
                if(enabledStart < 0)
                    enabledStart = i;
                enabledEnd = i;
            }
            #endif

            numBoxes++;
         }
      }

	  if(num > 1) {
		  //find closest point
		  double smallestDist = 100000;
		  double endPoint1[3];
		  double endPoint2[3];

		  for(int i = 0; i < obj[0].VertexCount; i++) {
		  	for(int j = 0; j < obj[1].VertexCount; j++) {
		  		double dist = GlobalFunctions::EuclideanDistance(obj[0].Vertices[i], obj[1].Vertices[j]);
		  		if(dist < smallestDist) {
		  			smallestDist = dist;
		  			for(int k = 0; k < 3; k++) {
			  			endPoint1[k] = obj[0].Vertices[i][k];
			  			endPoint2[k] = obj[1].Vertices[j][k];
		  			}
	  			}
		  	}
	  	  }
	  	  dMatrix3 R;
	   	  obj[num].body = dBodyCreate (world);
		  dBodySetPosition (obj[num].body, (endPoint1[0] + endPoint2[0])/2.0,(endPoint1[1] + endPoint2[1])/2.0,(endPoint1[2] + endPoint2[2])/2.0);
		  dBodySetData (obj[num].body,(void*)(size_t)num);
		  dMass m2;
		  dMassSetCapsule (&m2,5.0, 3, 0.1, smallestDist);
		  obj[num].geom = dCreateCapsule (space,0.1,smallestDist);
		  dGeomSetBody (obj[num].geom,obj[num].body);
		  dBodySetMass (obj[num].body,&m2);

	  	  double axis1[] = {endPoint1[0] - endPoint2[0],endPoint1[1] - endPoint2[1],endPoint1[2] - endPoint2[2]};
	  	  cout << axis1[0] << " " << axis1[1] << " " << axis1[2] << endl;

	  	  double axis2[3];
	  	  bool all0 = true;
	  	  if(axis1[0] == 0) {
	  	  	axis2[0] = 1;
	  	  	all0 = false;
	  	  } else
	  	  	axis2[0] = 0;

	  	  if(axis1[1] == 0) {
	  	  	axis2[1] = 1;
	  	  	all0 = false;
	  	  } else
	  	  	axis2[1] = 0;

	  	  if(axis1[2] == 0) {
	  	  	axis2[2] = 1;
	  	  	all0 = false;
	  	  } else
	  	  	axis2[2] = 0;
	  	  if(all0)
	  	  	axis2[0] = 1;
	  	  double *cross = GlobalFunctions::CrossProduct(axis2,axis1);
	  	  cout << cross[0] << " " << cross[1] << " " << cross[2] << endl;
	  	  dRFrom2Axes (R, cross[0], cross[1], cross[2], axis2[0], axis2[1], axis2[2]);
	  	  delete[] cross;
	  	  dBodySetRotation(obj[num].body, R);
		  num++;

		  joint[numJoints].joint = dJointCreateHinge(world,0);
		  dJointAttach(joint[numJoints].joint, obj[0].body, obj[2].body);
	  	  dJointSetHingeAnchor(joint[numJoints].joint,endPoint1[0], endPoint1[1], endPoint1[2]);
	  	  dJointSetHingeAxis(joint[numJoints].joint,  endPoint1[0] - endPoint2[0],endPoint1[1] - endPoint2[1],endPoint1[2] - endPoint2[2]);
	  	  dJointSetHingeParam(joint[numJoints].joint,dParamLoStop,-PI/2.0);
		  dJointSetHingeParam(joint[numJoints].joint,dParamHiStop,PI/2.0);
	  	  numJoints++;

		  joint[numJoints].joint = dJointCreateHinge(world,0);
		  dJointAttach(joint[numJoints].joint, obj[1].body, obj[2].body);
	  	  dJointSetHingeAnchor(joint[numJoints].joint,endPoint2[0], endPoint2[1], endPoint2[2]);
	  	  dJointSetHingeAxis(joint[numJoints].joint, 0,0,-1);
	  	  dJointSetHingeParam(joint[numJoints].joint,dParamLoStop,-PI/2.0);
		  dJointSetHingeParam(joint[numJoints].joint,dParamHiStop,PI/2.0);
	  	  numJoints++;
	  } else {
	  	#if REFLECT_AND_COPY
		  obj[num].body = dBodyCreate (world);
		  dBodySetPosition (obj[num].body, 0,0,0);
		  dBodySetData (obj[num].body,(void*)(size_t)num);

		  obj[num].VertexCount = obj[0].VertexCount;
		  obj[num].Vertices = new dVector3[obj[num].VertexCount];
		  for(int j=0; j<obj[num].VertexCount; j++) {
			obj[num].Vertices[j][0] = obj[0].Vertices[j][0];
			obj[num].Vertices[j][1] = obj[0].Vertices[j][1];//-1.0 * obj[0].Vertices[j][1];
			obj[num].Vertices[j][2] = -1.0 * obj[0].Vertices[j][2];
		  }

		  obj[num].IndexCount = obj[0].IndexCount;
		  obj[num].Indices = new dTriIndex[obj[num].IndexCount];
		  for(int j=0; j< obj[num].IndexCount; j+=3) {
			obj[num].Indices[j] = obj[0].Indices[j+2];
			obj[num].Indices[j+1] = obj[0].Indices[j+1];
			obj[num].Indices[j+2] = obj[0].Indices[j];
		  }
		  dTriMeshDataID new_tmdata = dGeomTriMeshDataCreate();
		  dGeomTriMeshDataBuildSimple(new_tmdata, (dReal*) obj[num].Vertices, obj[num].VertexCount, obj[num].Indices, obj[num].IndexCount);
		  obj[num].geom = dCreateTriMesh(space, new_tmdata, 0, 0, 0);

		  // remember the mesh's dTriMeshDataID on its userdata for convenience.
		  dGeomSetData(obj[num].geom, new_tmdata);

		  dMass m;
		  dMassSetTrimesh( &m, DENSITY, obj[num].geom );
		  printf("mass of %f at %f %f %f\n", m.mass, m.c[0], m.c[1], m.c[2]);
		  dGeomSetPosition(obj[num].geom, -m.c[0], -m.c[1], -m.c[2]);
		  dMassTranslate(&m, -m.c[0], -m.c[1], -m.c[2]);

		  dGeomSetBody (obj[num].geom,obj[num].body);
		  dBodySetMass (obj[num].body,&m);
		  num++;


		  double smallest[] = {1000, 1000, 1000};
		  int smallestIndex[] = {-1, -1, -1};
		  double biggest[] = {-1000, -1000, -1000};
		  int biggestIndex[] = {-1, -1, -1};

		  for(int i=0; i<obj[0].VertexCount; i++) {
		  	for(int j=0; j<3; j++) {
		  		if(obj[0].Vertices[i][j] < smallest[j]) {
		  			smallest[j] = obj[0].Vertices[i][j];
		  			smallestIndex[j] = i;
		  		}
		  		if(obj[0].Vertices[i][j] > biggest[j]) {
		  			biggest[j] = obj[0].Vertices[i][j];
		  			biggestIndex[j] = i;
		  		}
		  	}

		  }

		  cout << obj[0].Vertices[smallestIndex[2]][0] << " " << obj[0].Vertices[smallestIndex[2]][1] << " " << obj[0].Vertices[smallestIndex[2]][2] << endl;

		  cout << biggest[1] << endl;

		  dMatrix3 R;
	  	  dRFromAxisAndAngle (R,1,0,0,PI/2.0);
		  dBodySetRotation(obj[0].body, R);
		  dBodySetRotation(obj[1].body, R);

		  dBodySetPosition(obj[0].body, 0,-0.1,-1.0*smallest[1] + boxHeight);// + 0.2);// + 2);
		  dBodySetPosition(obj[1].body, 0,0.1,-1.0*smallest[1] + boxHeight);// + 0.2);// + 2);


	   	  obj[num].body = dBodyCreate (world);
		  dBodySetPosition (obj[num].body, obj[0].Vertices[smallestIndex[2]][0], 0, obj[0].Vertices[smallestIndex[2]][1] - smallest[1] + boxHeight);// + 0.2);
		  dBodySetData (obj[num].body,(void*)(size_t)num);
		  dMass m2;
		  dMassSetCapsule (&m2,5.0, 2, 0.1, 0.2);
		  obj[num].geom = dCreateCapsule (space,0.1,0.2);
		  dGeomSetBody (obj[num].geom,obj[num].body);
		  dBodySetMass (obj[num].body,&m2);
	  	  dRFromAxisAndAngle (R,1,0,0,-PI/2.0);
	  	  dBodySetRotation(obj[num].body, R);
		  num++;




		  joint[numJoints].joint = dJointCreateHinge(world,0);
		  dJointAttach(joint[numJoints].joint, obj[0].body, obj[2].body);
	  	  dJointSetHingeAnchor(joint[numJoints].joint,obj[0].Vertices[smallestIndex[2]][0],-0.1, obj[0].Vertices[smallestIndex[2]][1] - 1.0*smallest[1] + boxHeight);
	  	  
	  	  double joint1Normal[] = {1, 0, 0};
	  	  if( (params.count("Joint1X") > 0) && (params.count("Joint1Y") > 0) && (params.count("Joint1Z") > 0)) {
	  	  	joint1Normal[0] = params["Joint1X"];
	  	  	joint1Normal[1] = params["Joint1Y"];
	  	  	joint1Normal[2] = params["Joint1Z"];	  	  		  	  	
	  	  }
	  	  //dJointSetHingeAxis(joint[numJoints].joint, obj[0].Vertices[smallestIndex[2]][0],0,1);
	  	  dJointSetHingeAxis(joint[numJoints].joint, joint1Normal[0], joint1Normal[1], joint1Normal[2]);
	  	  cout << "Joint 1, axis " << joint1Normal[0] << " " << joint1Normal[1] << " " << joint1Normal[2] << endl;
	  	  dJointSetHingeParam(joint[numJoints].joint,dParamLoStop,params["Amplitude1"]*-PI);
		  dJointSetHingeParam(joint[numJoints].joint,dParamHiStop,params["Amplitude1"]*PI);
		  joint[numJoints].period = params["Period1"];
		  joint[numJoints].phaseShift = params["PhaseShift1"];
	  	  numJoints++;

		  joint[numJoints].joint = dJointCreateHinge(world,0);
		  dJointAttach(joint[numJoints].joint, obj[1].body, obj[2].body);
	  	  dJointSetHingeAnchor(joint[numJoints].joint,obj[0].Vertices[smallestIndex[2]][0],0.1, obj[0].Vertices[smallestIndex[2]][1] - 1.0*smallest[1] + boxHeight);
	  	  double joint2Normal[] = {0, 0, -1};
	  	  if( (params.count("Joint2X") > 0) && (params.count("Joint2Y") > 0) && (params.count("Joint2Z") > 0)) {
	  	  	joint2Normal[0] = params["Joint2X"];
	  	  	joint2Normal[1] = params["Joint2Y"];
	  	  	joint2Normal[2] = params["Joint2Z"];	  	  		  	  	
	  	  }  	  
	  	  //dJointSetHingeAxis(joint[numJoints].joint, 0,0,-1);
  	  	  dJointSetHingeAxis(joint[numJoints].joint, joint2Normal[0], joint2Normal[1], joint2Normal[2]);
	  	  cout << "Joint 2, axis " << joint2Normal[0] << " " << joint2Normal[1] << " " << joint2Normal[2] << endl;
	  	  dJointSetHingeParam(joint[numJoints].joint,dParamLoStop,params["Amplitude2"]*-PI);
		  dJointSetHingeParam(joint[numJoints].joint,dParamHiStop,params["Amplitude2"]*PI);
		  joint[numJoints].period = params["Period2"];
		  joint[numJoints].phaseShift = params["PhaseShift2"];
	  	  numJoints++;
		#endif
	  }

	  
	  t = 0;
	  evaluating = true;
	  evaluationCanceled = false;
	  cout << "evaluating\n";

	  // run simulation
	  if ( rendering ) {
		dsSimulationLoop(argc,argv,352*2,288*2,&fn);
	  } else {
		while ( evaluating ) {
			Tick(false,false);
		}
	  }
	  
	  
	  

	  ofstream *out = new ofstream(tempFileName);
	  if(evaluationCanceled) {
	        printf("eval canceled\n");
            (*out) << "0" << endl;
	  } else {
	        (*out) << positions.size() << endl;
	        (*out) << entropy << endl;
                for(int i=0; i<positions.size(); i++) {
                    vector<double> com = positions[i];
                    (*out) << com[0];
                    for(int j=1; j<com.size(); j++) {
	                    (*out) << " " << com[j];
                    }
                    (*out) << endl;
                }
      }
      out->close();
      delete out;

  	  GlobalFunctions::FileDelete(indicesFileName);
      GlobalFunctions::FileDelete(verticesFileName);
      GlobalFunctions::FileDelete(paramsFileName);

	  GlobalFunctions::FileMove(tempFileName, resultsFileName);
	  GlobalFunctions::FileTouch(readyFileName);	  

      positions.clear();
      angVels.clear();
      linVels.clear();

	  destroyODE();
      printf("done evaluating\n");
      if(runOnce)
	     exit(0);

  }
  if(contactsOut != NULL) {
  	contactsOut->close();
  	delete contactsOut;
  }

  return 0;
}



