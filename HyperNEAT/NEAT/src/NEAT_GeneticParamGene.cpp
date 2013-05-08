#include "NEAT_GeneticParamGene.h"

#include "NEAT_Random.h"
#include "NEAT_Globals.h"

#include <iomanip>

namespace NEAT
{

    GeneticParamGene::GeneticParamGene(const string &_name,double _minValue, double _maxValue)
        :
        GeneticGene(),
        name(_name),
        minValue(_minValue),
        maxValue(_maxValue)
    {
        value = Globals::getSingleton()->getRandom().getRandomDouble(minValue,maxValue);
        
        
        Globals::getSingleton()->assignParamID(this);
    }


    GeneticParamGene::GeneticParamGene(TiXmlElement *paramElementPtr)
            :
            GeneticGene(paramElementPtr)
    {
        name = paramElementPtr->Attribute("Name");
        value = atof(paramElementPtr->Attribute("Value"));
        minValue = atof(paramElementPtr->Attribute("MinValue"));
        maxValue = atof(paramElementPtr->Attribute("MaxValue"));

    }

    GeneticParamGene::GeneticParamGene(istream &istr)
            :
            GeneticGene(istr)
    {
        istr >> name >> value >> minValue >> maxValue;
        if (name==string("__NO_NAME__"))
        {
            name = string("");
        }
    }


    GeneticParamGene::~GeneticParamGene()
    {}

    bool GeneticParamGene::operator==(const GeneticParamGene &other) const
    {
        return
            (
                GeneticGene::operator==(other) &&
                value == other.value &&
                minValue == other.minValue &&
                maxValue == other.maxValue &&
                name == other.name
            );
    }




    void GeneticParamGene::mutate()
    {
        //double mutationPower = Globals::getSingleton()->getParameterValue("MutationPower");
        double mutationPower = Globals::getSingleton()->getParameterValue("MutationPower") * (maxValue - minValue) / 10.0;
        value += mutationPower*(2.0*(Globals::getSingleton()->getRandom().getRandomDouble()-0.5));

        if (value>maxValue)
            value=maxValue;
        else if (value<minValue)
            value=minValue;

    }

    void GeneticParamGene::dump(TiXmlElement *XMLnode)
    {
        GeneticGene::dump(XMLnode);
        XMLnode->SetAttribute("Name",name);
        XMLnode->SetDoubleAttribute("Value",value);
        XMLnode->SetDoubleAttribute("MinValue",minValue);
        XMLnode->SetDoubleAttribute("MaxValue",maxValue);
    }

    void GeneticParamGene::dump(ostream &ostr)
    {
        GeneticGene::dump(ostr);

        string tmpName = name;

        if (tmpName.length()==0)
        {
            tmpName = string("__NO_NAME__");
        }

        ostr << tmpName << ' ' << value << ' ' << minValue << ' ' << maxValue << ' ';
    }
}

