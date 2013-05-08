#ifndef _GLOBAL_FUNCTIONS_CPP
#define _GLOBAL_FUNCTIONS_CPP

#include "GlobalFunctions.h"


#include <ode/odemath.h>
#include "fstream"


using namespace std;



double GlobalFunctions::EuclideanDistance(const double a[3], const double b[3]) {
    return ( sqrtf(powf(a[0] - b[0],2.0) + powf(a[1] - b[1],2.0) + powf(a[2] - b[2],2.0)) );
}

double GlobalFunctions::EuclideanDistanceSquared(const double a[3], const double b[3]) {
    return ( powf(a[0] - b[0],2.0) + powf(a[1] - b[1],2.0) + powf(a[2] - b[2],2.0) );
}

double* GlobalFunctions::CrossProduct(const double a[3], const double b[3]) {
	double *result = new double[3];
	result[0] = (a[1] * b[2] - a[2] * b[1]);
	result[1] = (a[2] * b[0] - a[0] * b[2]);
	result[2] = (a[0] * b[1] - a[1] * b[0]);
	return result;
}


void  GlobalFunctions::FileCopy(char *src, char *dest) {

    char command[200];

    sprintf(command,"cp %s %s", src, dest);

    system(command);
}

void  GlobalFunctions::FileMove(char *src, char *dest) {

    char command[200];

    sprintf(command,"mv %s %s", src, dest);

    system(command);
}

void  GlobalFunctions::FileDelete(char *fileName) {

    char command[200];

    sprintf(command,"rm %s",fileName);

    system(command);
}

int   GlobalFunctions::FileExists(char *fileName) {

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

void  GlobalFunctions::FileTouch(char *fileName) {

    char command[200];

    sprintf(command,"touch %s",fileName);

    system(command);
}


bool GlobalFunctions::FlipCoin(void) {
    return (Rand(0,1) < 0.5);
}

char GlobalFunctions::Locase (char c) {
  if (c >= 'A' && c <= 'Z') return c - ('a'-'A');
  else return c;
}

double GlobalFunctions::Rand(double min, double max) {

	double zeroToOne = ((double) rand() ) / RAND_MAX;
	double returnVal;

	returnVal = (zeroToOne * (max-min)) + min;
	return returnVal;
}

double GlobalFunctions::RandGaussian(void) {

	double w = 1.01;
	double x1, x2;

	while ( w >= 1.0 ) {
		x1 = 2.0*Rand(0,1) - 1;
		x2 = 2.0*Rand(0,1) - 1;
		w = x1*x1 + x2*x2;
	}
	w = sqrt( (-2.0*log(w))/w );

	return( x1*w );
}

double GlobalFunctions::RandGaussian(double mean, double std) {
	return ((RandGaussian()*std)+mean);
}

int GlobalFunctions::RandInt(int min, int max) {
 	if ( min == max )
		return( min );
	else
		return( (rand() % (max-min+1)) + min );

}

double GlobalFunctions::VectorLength(const double *myVector, int size) {
    double sumOfSquares = 0.0;
    for(int i = 0; i < size; i++) {
        sumOfSquares += myVector[i] * myVector[i];
    }
    return sqrt(sumOfSquares);
}

#endif
