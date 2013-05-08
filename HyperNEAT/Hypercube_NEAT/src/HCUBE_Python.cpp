#include "HCUBE_Defines.h"

#include "HCUBE_ExperimentRun.h"

#include <boost/python.hpp>
using namespace boost::python;

extern int HyperNEAT_main(int argc,char **argv);

int PythonMain(python::list args)
{
    int argc = python::len(args);

    char** argv = new char*[argc];

    for(int a=0;a<argc;a++)
    {
        argv[a] = python::extract<char*>(args[a]);
    }

    return HyperNEAT_main(argc,argv);
}

void setup(string file,string outputFile)
{
    cout << "LOADING GLOBALS FROM FILE: " << file << endl;
    cout << "OUTPUT FILE: " << outputFile << endl;
    NEAT::Globals::init(file);

    int experimentType = int(NEAT::Globals::getSingleton()->getParameterValue("ExperimentType")+0.001);

    cout << "Loading Experiment: " << experimentType << endl;

    HCUBE::ExperimentRun experimentRun;

    experimentRun.setupExperiment(experimentType,outputFile);

    cout << "Experiment set up\n";

    experimentRun.createPopulation();

    experimentRun.setCleanup(true);

    cout << "Population Created\n";

    experimentRun.start();
}

BOOST_PYTHON_MODULE(HCUBE)
{
    def("HyperNEAT_main", PythonMain);
    def("setup", setup);
}
