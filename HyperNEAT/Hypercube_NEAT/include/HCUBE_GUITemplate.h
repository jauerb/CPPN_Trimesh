///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Mar 15 2007)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __HCUBE_GUITemplate__
#define __HCUBE_GUITemplate__

#include <wx/wx.h>

namespace HCUBE
{
    class EvaluationPanel;
}
namespace HCUBE
{
    class NetworkPanel;
}
namespace HCUBE
{
    class NeuralNetworkPanel;
}
namespace HCUBE
{
    class NeuralNetworkWeightGrid;
}

#include <wx/menu.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/tglbtn.h>
#include <wx/grid.h>

///////////////////////////////////////////////////////////////////////////

namespace HCUBE
{
///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameTemplate
///////////////////////////////////////////////////////////////////////////////
    class MainFrameTemplate : public wxFrame
    {
        DECLARE_EVENT_TABLE()
    private:

        // Private event handlers
        void _wxFB_loadExperiment( wxCommandEvent& event )
        {
            loadExperiment( event );
        }
        void _wxFB_runExperiment( wxCommandEvent& event )
        {
            runExperiment( event );
        }
        void _wxFB_restartExperiment( wxCommandEvent& event )
        {
            restartExperiment( event );
        }
        void _wxFB_pauseExperiment( wxCommandEvent& event )
        {
            pauseExperiment( event );
        }
        void _wxFB_loadPopulation( wxCommandEvent& event )
        {
            loadPopulation( event );
        }
        void _wxFB_exit( wxCommandEvent& event )
        {
            exit( event );
        }
        void _wxFB_about( wxCommandEvent& event )
        {
            about( event );
        }
        void _wxFB_viewIndividual( wxCommandEvent& event )
        {
            viewIndividual( event );
        }
        void _wxFB_analyzeIndividual( wxCommandEvent& event )
        {
            analyzeIndividual( event );
        }
        void _wxFB_rerunAll( wxCommandEvent& event )
        {
            rerunAll( event );
        }


    protected:
        enum
        {
            wxID_LOADEXPERIMENT_MENUITEM = 5000,
            wxID_RUNEXPERIMENT_MENUITEM,
            wxID_RESTARTEXPERIMENT_MENUITEM,
            wxID_PAUSEEXPERIMENT_MENUITEM,
            wxID_LOADPOPULATION_MENUITEM,
            wxID_EXIT_MENUITEM,
            wxID_ABOUT_MENUITEM,
            wxID_GENERATION_SPINNER,
            wxID_INDIVIDUAL_SPINNER,
            wxID_VIEWINDIVIDUAL_BUTTON,
            wxID_ANALYZEINDIVIDUAL_BUTTON,
            wxID_RERUNALL_BUTTON,
            wxID_PARAMETER_LISTBOX,
            wxID_CONSOLEOUTPUT_TEXTBOX,
        };

        wxStatusBar* m_statusBar1;
        wxMenuBar* menubar;
        wxMenu* fileMenu;
        wxMenu* helpMenu;
        wxSpinCtrl* generationSpinner;
        wxSpinCtrl* individualSpinner;
        wxButton* viewIndividualButton;
        wxButton* analyzeIndividualButton;
        wxButton* rerunAllButton;
        wxNotebook* m_notebook3;
        wxPanel* parametersPage;
        wxListBox* parameterListBox;
        wxPanel* outputLogPage;
        wxTextCtrl* consoleOutputTextBox;

        // Virtual event handlers, overide them in your derived class
        virtual void loadExperiment( wxCommandEvent& event )
        {
            event.Skip();
        }
        virtual void runExperiment( wxCommandEvent& event )
        {
            event.Skip();
        }
        virtual void restartExperiment( wxCommandEvent& event )
        {
            event.Skip();
        }
        virtual void pauseExperiment( wxCommandEvent& event )
        {
            event.Skip();
        }
        virtual void loadPopulation( wxCommandEvent& event )
        {
            event.Skip();
        }
        virtual void exit( wxCommandEvent& event )
        {
            event.Skip();
        }
        virtual void about( wxCommandEvent& event )
        {
            event.Skip();
        }
        virtual void viewIndividual( wxCommandEvent& event )
        {
            event.Skip();
        }
        virtual void analyzeIndividual( wxCommandEvent& event )
        {
            event.Skip();
        }
        virtual void rerunAll( wxCommandEvent& event )
        {
            event.Skip();
        }


    public:
        MainFrameTemplate( wxWindow* parent, int id = wxID_ANY, wxString title = wxT("Jason Gauci's HyperNEAT"), wxPoint pos = wxDefaultPosition, wxSize size = wxSize( 640,480 ), int style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

    };

///////////////////////////////////////////////////////////////////////////////
/// Class UserEvaluationFrameTemplate
///////////////////////////////////////////////////////////////////////////////
    class UserEvaluationFrameTemplate : public wxFrame
    {
        DECLARE_EVENT_TABLE()
    private:

        // Private event handlers
        void _wxFB_advanceGeneration( wxCommandEvent& event )
        {
            advanceGeneration( event );
        }
        void _wxFB_resetVotes( wxCommandEvent& event )
        {
            resetVotes( event );
        }


    protected:
        HCUBE::EvaluationPanel* evaluationPanel0;
        HCUBE::EvaluationPanel* evaluationPanel1;
        HCUBE::EvaluationPanel* evaluationPanel2;
        HCUBE::EvaluationPanel* evaluationPanel3;
        HCUBE::EvaluationPanel* evaluationPanel4;
        HCUBE::EvaluationPanel* evaluationPanel5;
        HCUBE::EvaluationPanel* evaluationPanel6;
        HCUBE::EvaluationPanel* evaluationPanel7;
        wxNotebook* m_notebook1;
        HCUBE::EvaluationPanel* highResPanel;
        HCUBE::NeuralNetworkPanel* networkPanel;
        wxPanel* votingPanel;
        wxToggleButton* voteButton0;
        wxToggleButton* voteButton1;
        wxToggleButton* voteButton2;
        wxToggleButton* voteButton3;
        wxToggleButton* voteButton4;
        wxToggleButton* voteButton5;
        wxToggleButton* voteButton6;
        wxToggleButton* voteButton7;
        wxButton* advanceGenerationButton;
        wxButton* resetVotesButton;

        // Virtual event handlers, overide them in your derived class
        virtual void advanceGeneration( wxCommandEvent& event )
        {
            event.Skip();
        }
        virtual void resetVotes( wxCommandEvent& event )
        {
            event.Skip();
        }


    public:
        UserEvaluationFrameTemplate( wxWindow* parent, int id = wxID_ANY, wxString title = wxT("User Evaluations"), wxPoint pos = wxDefaultPosition, wxSize size = wxSize( 800,600 ), int style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

    };

///////////////////////////////////////////////////////////////////////////////
/// Class NeuralNetworkPanelTemplate
///////////////////////////////////////////////////////////////////////////////
    class NeuralNetworkPanelTemplate : public wxPanel
    {
    private:

    protected:
        HCUBE::NetworkPanel* networkVisualizationPanel;
        HCUBE::NeuralNetworkWeightGrid* networkAdjacency;

    public:
        NeuralNetworkPanelTemplate( wxWindow* parent, int id = wxID_ANY, wxPoint pos = wxDefaultPosition, wxSize size = wxSize( 512,512 ), int style = wxTAB_TRAVERSAL );

    };

///////////////////////////////////////////////////////////////////////////////
/// Class ViewIndividualFrameTemplate
///////////////////////////////////////////////////////////////////////////////
    class ViewIndividualFrameTemplate : public wxFrame
    {
    private:

    protected:
        wxNotebook* m_notebook2;
        HCUBE::EvaluationPanel* evaluationPanel;
        HCUBE::NeuralNetworkPanel* networkPanel;

    public:
        ViewIndividualFrameTemplate( wxWindow* parent, int id = wxID_ANY, wxString title = wxEmptyString, wxPoint pos = wxDefaultPosition, wxSize size = wxSize( 640,480 ), int style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

    };

///////////////////////////////////////////////////////////////////////////////
/// Class TicTacToeGameExperimentPanelTemplate
///////////////////////////////////////////////////////////////////////////////
    class TicTacToeGameExperimentPanelTemplate : public wxPanel
    {
    private:

    protected:

    public:
        TicTacToeGameExperimentPanelTemplate( wxWindow* parent, int id = wxID_ANY, wxPoint pos = wxDefaultPosition, wxSize size = wxSize( 500,300 ), int style = wxTAB_TRAVERSAL );

    };

} // namespace HCUBE

#endif //__HCUBE_GUITemplate__
