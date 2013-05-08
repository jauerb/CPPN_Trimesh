#ifndef HCUBE_EVALUATIONPANEL_H_INCLUDED
#define HCUBE_EVALUATIONPANEL_H_INCLUDED

#include "HCUBE_Defines.h"

namespace HCUBE
{
    class EvaluationPanel : public wxPanel
    {
    public:
    protected:
        shared_ptr<Experiment> experiment,backupExperiment;
        shared_ptr<NEAT::GeneticIndividual> individual,backupIndividual;
        wxBitmap *outputBitmap;
        bool bitmapLocked;

    public:
        EvaluationPanel(
            wxWindow* parent,
            int id = wxID_ANY,
            wxPoint pos = wxDefaultPosition,
            wxSize size = wxDefaultSize,
            int style = wxTAB_TRAVERSAL
        );

        virtual ~EvaluationPanel()
        {
            delete outputBitmap;
        }

        /*
        EvaluationPanel(
        Experiment *_experiment,
        shared_ptr<NEAT::GeneticIndividual> _individual,
        wxWindow* _parent=NULL,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize
        );
        */

        inline shared_ptr<NEAT::GeneticIndividual> getIndividual()
        {
            return individual;
        }

        void setTarget(shared_ptr<Experiment> _experiment,shared_ptr<NEAT::GeneticIndividual> _individual);

        void paint(wxPaintEvent& event);

        void handleMouseClick(wxMouseEvent& event);

        void handleMouseMotion(wxMouseEvent& event);

        DECLARE_EVENT_TABLE()

    protected:
        //Get rid of default copy ctor & assignment
        EvaluationPanel(const EvaluationPanel &other)
        {}

        const EvaluationPanel &operator=(const EvaluationPanel &other)
        {
            return *this;
        }
    };
}


#endif // HCUBE_EVALUATIONPANEL_H_INCLUDED
