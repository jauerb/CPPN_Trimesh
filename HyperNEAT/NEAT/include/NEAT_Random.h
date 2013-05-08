#ifndef __NEAT_RANDOM_H__
#define __NEAT_RANDOM_H__

#include "NEAT_Defines.h"

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/random/normal_distribution.hpp>

typedef boost::minstd_rand base_generator_type;
// Define a uniform random number distribution of integer values between
// 1 and 6 inclusive.
typedef boost::uniform_smallint<> int_distribution_type;

typedef boost::uniform_real<> real_distribution_type;

typedef boost::normal_distribution<> normal_distribution_type;

namespace NEAT
{
    class Random
    {
    public:

    protected:
        unsigned int seed;

        base_generator_type generator;

        int_distribution_type intDist;
        variate_generator<base_generator_type,int_distribution_type> intGen;

        real_distribution_type realDist;
        variate_generator<base_generator_type,real_distribution_type> realGen;

        normal_distribution_type normalDist;
        variate_generator<base_generator_type,normal_distribution_type> normalGen;

    public:
        /**
         * Constructor:
         * Passing 0 for 'seed' sets the seed to the current time value
         */
        NEAT_DLL_EXPORT Random(unsigned int _seed=0);

        ///Gets a random int 0 <= x < limit;
        NEAT_DLL_EXPORT int getRandomInt(int limit);

        ///Gets a random int min <= x <= max;
        NEAT_DLL_EXPORT int getRandomWithinRange(int min,int max);

        ///Gets a random int min <= x <= max;
        ///Gets a random floating point number low <= x <= high
        NEAT_DLL_EXPORT double getRandomDouble();

        NEAT_DLL_EXPORT double getRandomDouble(double low,double high);
        
        NEAT_DLL_EXPORT double getRandomGaussian(double mean, double stdDev);

        inline unsigned int getSeed()
        {
            return seed;
        }
    protected:
    };
}

/*
#include "NEAT_Defines.h"
#include "NEAT_STL.h"

namespace NEAT
{

    class Random
    {
    protected:
        unsigned int seed;

        NEAT_DLL_EXPORT static Random *singleton;
    public:
        static inline Random *getInstance()
        {
            if(!singleton)
                cout << "Singleton class not defined!" << endl;

            return singleton;
        }

        static void init(unsigned int seed)
        {
            if(singleton)
                delete singleton;

            singleton = new Random(seed);
        }

        static void init()
        {
            singleton = new Random();
        }

        static void deinit()
        {
            delete singleton;
        }

        ///Gets a random int 0 <= x < limit;
        inline int getRandomInt(int limit)
        {
            int lowest=0, highest=limit;
            int range=(highest-lowest);
            return lowest+int(range*(rand()/1000)/((RAND_MAX/1000) + 1.0));
        }
        ///Gets a random int min <= x <= max;
        inline int getRandomWithinRange(int min,int max)
        {
            if(min==max)
                return min;
            int lowest=min, highest=max;
            int range=(highest-lowest)+1;
            int value = lowest+int(range*(rand()/1000)/((RAND_MAX/1000) + 1.0));
            return value;
        }
        ///Gets a random int min <= x <= max;
        ///Gets a random floating point number low <= x <= high
        inline double getRandomDouble()
        {
            return double(rand())/RAND_MAX;
        }
        inline double getRandomDouble(int low,int high)
        {
            return (double(rand())/RAND_MAX*(high-low))+low;
        }
        inline double getRandomDouble(double low,double high)
        {
            return (double(rand())/RAND_MAX*(high-low))+low;
        }
        inline unsigned int getSeed()
        {
            return seed;
        }
    protected:
        Random(unsigned int _seed =
            (static_cast<unsigned int>(std::clock()%1000000))
            + (static_cast<unsigned int>(time(NULL)%1000000)))
                :
                seed(_seed)
        {
            srand(seed);
        }

        virtual ~Random()
        {}
    };

}
*/

#endif

