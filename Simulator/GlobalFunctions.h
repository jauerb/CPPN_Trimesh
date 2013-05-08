#ifndef _GLOBAL_FUNCTIONS_H
#define _GLOBAL_FUNCTIONS_H

#define PI (3.14159)

#define EPSILON 0.0001
#define MAX_DENSITY (1.0)

using namespace std;


enum SensorType {
  SENSOR_TYPE_DISTANCE = 0,
  SENSOR_TYPE_TOUCH,
  SENSOR_TYPE_ANGLE,
  SENSOR_TYPE_TIME,
  SENSOR_TYPE_DISTANCE_AND_TIME,
  SENSOR_TYPE_END
};

class GlobalFunctions {


public:



static double EuclideanDistance(const double a[3], const double b[3]);
static double EuclideanDistanceSquared(const double a[3], const double b[3]);
static double* CrossProduct(const double a[3], const double b[3]);

static void  FileCopy(char *src, char *dest);
static void  FileMove(char *src, char *dest);
static void  FileDelete(char *fileName);
static int   FileExists(char *fileName);
static void  FileTouch(char *fileName);

static bool FlipCoin(void);

static char Locase (char c);

static double Rand(double min, double max);

static double RandGaussian(void);

static double RandGaussian(double mean, double std);

static int RandInt(int min, int max);

static double VectorLength(const double *myVector, int size);



};

#endif
