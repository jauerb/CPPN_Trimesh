
#ifndef __GENETICPARAMGENE_H__
#define __GENETICPARAMGENE_H__

#include "NEAT_GeneticGene.h"

#include "NEAT_Globals.h"

#include "NEAT_Random.h"

namespace NEAT
{
    /**
     * GeneticParamGene: This gene contains a parameter value
     */
    class NEAT_DLL_EXPORT GeneticParamGene : public GeneticGene
    {
    protected:
        string name;
        double value, minValue, maxValue;


    public:
        GeneticParamGene(const string &_name,double minValue, double maxValue);

        virtual ~GeneticParamGene();

        GeneticParamGene(TiXmlElement *paramElementPtr);

        GeneticParamGene(istream &istr);

        virtual bool operator==(const GeneticParamGene &other) const;

        inline const string &getName() const
        {
            return name;
        }



        virtual void mutate();

        virtual void dump(TiXmlElement *XMLnode);

        virtual void dump(ostream &ostr);

        inline double getValue() const
        {
            return value;
        }

        inline void setValue(double _value)
        {
            value = _value;
        }

        inline double getMinValue() const
        {
            return minValue;
        }

        inline void setMinValue(double _minValue)
        {
            minValue = _minValue;
        }

        inline double getMaxValue() const
        {
            return maxValue;
        }

        inline void setMaxValue(double _maxValue)
        {
            maxValue = _maxValue;
        }


    };

}

#endif
