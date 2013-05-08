#include "HCUBE_Defines.h"

// C++ code generated with wxFormBuilder (version Mar 15 2007)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////
#ifndef HCUBE_NOGUI

#ifdef WX_PRECOMP

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#else
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "HCUBE_EvaluationPanel.h"
#include "HCUBE_NetworkPanel.h"
#include "HCUBE_NeuralNetworkPanel.h"
#include "HCUBE_NeuralNetworkWeightGrid.h"

#include "HCUBE_GUITemplate.h"

///////////////////////////////////////////////////////////////////////////
using namespace HCUBE;
BEGIN_EVENT_TABLE( MainFrameTemplate, wxFrame )
EVT_MENU( wxID_LOADEXPERIMENT_MENUITEM, MainFrameTemplate::_wxFB_loadExperiment )
EVT_MENU( wxID_RUNEXPERIMENT_MENUITEM, MainFrameTemplate::_wxFB_runExperiment )
EVT_MENU( wxID_RESTARTEXPERIMENT_MENUITEM, MainFrameTemplate::_wxFB_restartExperiment )
EVT_MENU( wxID_PAUSEEXPERIMENT_MENUITEM, MainFrameTemplate::_wxFB_pauseExperiment )
EVT_MENU( wxID_LOADPOPULATION_MENUITEM, MainFrameTemplate::_wxFB_loadPopulation )
EVT_MENU( wxID_EXIT_MENUITEM, MainFrameTemplate::_wxFB_exit )
EVT_MENU( wxID_ABOUT_MENUITEM, MainFrameTemplate::_wxFB_about )
EVT_BUTTON( wxID_VIEWINDIVIDUAL_BUTTON, MainFrameTemplate::_wxFB_viewIndividual )
EVT_BUTTON( wxID_ANALYZEINDIVIDUAL_BUTTON, MainFrameTemplate::_wxFB_analyzeIndividual )
EVT_BUTTON( wxID_RERUNALL_BUTTON, MainFrameTemplate::_wxFB_rerunAll )
END_EVENT_TABLE()

MainFrameTemplate::MainFrameTemplate( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxFrame( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    m_statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
    menubar = new wxMenuBar( 0 );
    fileMenu = new wxMenu();
    wxMenuItem* loadExperimentMenuItem = new wxMenuItem( fileMenu, wxID_LOADEXPERIMENT_MENUITEM, wxString( wxT("Load Experiment") ) , wxEmptyString, wxITEM_NORMAL );
    fileMenu->Append( loadExperimentMenuItem );
    wxMenuItem* runExperimentMenuItem = new wxMenuItem( fileMenu, wxID_RUNEXPERIMENT_MENUITEM, wxString( wxT("Run Experiment") ) , wxEmptyString, wxITEM_NORMAL );
    fileMenu->Append( runExperimentMenuItem );
    runExperimentMenuItem->Enable( false );

    wxMenuItem* restartExperimentMenuItem = new wxMenuItem( fileMenu, wxID_RESTARTEXPERIMENT_MENUITEM, wxString( wxT("Restart Experiment") ) , wxEmptyString, wxITEM_NORMAL );
    fileMenu->Append( restartExperimentMenuItem );
    restartExperimentMenuItem->Enable( false );

    wxMenuItem* pauseExperimentMenuItem = new wxMenuItem( fileMenu, wxID_PAUSEEXPERIMENT_MENUITEM, wxString( wxT("Pause Experiment") ) , wxEmptyString, wxITEM_NORMAL );
    fileMenu->Append( pauseExperimentMenuItem );
    pauseExperimentMenuItem->Enable( false );


    fileMenu->AppendSeparator();
    wxMenuItem* loadPopulationMenuItem = new wxMenuItem( fileMenu, wxID_LOADPOPULATION_MENUITEM, wxString( wxT("Load Population") ) , wxEmptyString, wxITEM_NORMAL );
    fileMenu->Append( loadPopulationMenuItem );

    fileMenu->AppendSeparator();
    wxMenuItem* exitMenuitem = new wxMenuItem( fileMenu, wxID_EXIT_MENUITEM, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
    fileMenu->Append( exitMenuitem );
    menubar->Append( fileMenu, wxT("File") );

    helpMenu = new wxMenu();
    wxMenuItem* aboutMenuItem = new wxMenuItem( helpMenu, wxID_ABOUT_MENUITEM, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
    helpMenu->Append( aboutMenuItem );
    menubar->Append( helpMenu, wxT("Help") );

    this->SetMenuBar( menubar );

    wxBoxSizer* bSizer6;
    bSizer6 = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* sbSizer13;
    sbSizer13 = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("Examine") ), wxHORIZONTAL );

    wxStaticBoxSizer* sbSizer5;
    sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("Generation") ), wxVERTICAL );

    generationSpinner = new wxSpinCtrl( this, wxID_GENERATION_SPINNER, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    sbSizer5->Add( generationSpinner, 1, wxALL|wxEXPAND, 5 );

    sbSizer13->Add( sbSizer5, 1, wxEXPAND, 5 );

    wxStaticBoxSizer* sbSizer51;
    sbSizer51 = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("Individual") ), wxVERTICAL );

    individualSpinner = new wxSpinCtrl( this, wxID_INDIVIDUAL_SPINNER, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    sbSizer51->Add( individualSpinner, 1, wxALL|wxEXPAND, 5 );

    sbSizer13->Add( sbSizer51, 1, wxEXPAND, 5 );

    wxStaticBoxSizer* sbSizer511;
    sbSizer511 = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("Commands") ), wxHORIZONTAL );

    viewIndividualButton = new wxButton( this, wxID_VIEWINDIVIDUAL_BUTTON, wxT("View"), wxDefaultPosition, wxDefaultSize, 0 );
    viewIndividualButton->Enable( false );

    sbSizer511->Add( viewIndividualButton, 0, wxALL|wxEXPAND, 5 );

    analyzeIndividualButton = new wxButton( this, wxID_ANALYZEINDIVIDUAL_BUTTON, wxT("Analyze"), wxDefaultPosition, wxDefaultSize, 0 );
    analyzeIndividualButton->Enable( false );

    sbSizer511->Add( analyzeIndividualButton, 0, wxALL|wxEXPAND, 5 );

    rerunAllButton = new wxButton( this, wxID_RERUNALL_BUTTON, wxT("Rerun"), wxDefaultPosition, wxDefaultSize, 0 );
    rerunAllButton->Enable( false );

    sbSizer511->Add( rerunAllButton, 0, wxALL|wxEXPAND, 5 );

    sbSizer13->Add( sbSizer511, 0, wxEXPAND, 5 );

    bSizer6->Add( sbSizer13, 0, wxEXPAND, 5 );

    m_notebook3 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
    parametersPage = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer8;
    bSizer8 = new wxBoxSizer( wxVERTICAL );

    parameterListBox = new wxListBox( parametersPage, wxID_PARAMETER_LISTBOX, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB|wxLB_HSCROLL|wxLB_SINGLE );
    bSizer8->Add( parameterListBox, 1, wxALL|wxEXPAND, 5 );

    parametersPage->SetSizer( bSizer8 );
    parametersPage->Layout();
    bSizer8->Fit( parametersPage );
    m_notebook3->AddPage( parametersPage, wxT("Parameters"), false );

    bSizer6->Add( m_notebook3, 2, wxEXPAND | wxALL, 5 );

    outputLogPage = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    outputLogPage->Hide();

    wxBoxSizer* bSizer9;
    bSizer9 = new wxBoxSizer( wxVERTICAL );

    consoleOutputTextBox = new wxTextCtrl( outputLogPage, wxID_CONSOLEOUTPUT_TEXTBOX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY );
    bSizer9->Add( consoleOutputTextBox, 1, wxALL|wxEXPAND, 5 );

    outputLogPage->SetSizer( bSizer9 );
    outputLogPage->Layout();
    bSizer9->Fit( outputLogPage );
    bSizer6->Add( outputLogPage, 1, wxEXPAND | wxALL, 5 );

    this->SetSizer( bSizer6 );
    this->Layout();
}
BEGIN_EVENT_TABLE( UserEvaluationFrameTemplate, wxFrame )
EVT_BUTTON( wxID_ANY, UserEvaluationFrameTemplate::_wxFB_advanceGeneration )
EVT_BUTTON( wxID_ANY, UserEvaluationFrameTemplate::_wxFB_resetVotes )
END_EVENT_TABLE()

UserEvaluationFrameTemplate::UserEvaluationFrameTemplate( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxFrame( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxSize( 800,600 ), wxDefaultSize );
    this->Centre( wxBOTH );

    wxBoxSizer* rootSizer;
    rootSizer = new wxBoxSizer( wxHORIZONTAL );

    wxGridSizer* evaluationSizer;
    evaluationSizer = new wxGridSizer( 4, 2, 0, 0 );

    evaluationSizer->SetMinSize( wxSize( 266,522 ) );
    evaluationPanel0 = new HCUBE::EvaluationPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 128,128 ), wxTAB_TRAVERSAL );
    evaluationPanel0->SetMinSize( wxSize( 128,128 ) );

    evaluationSizer->Add( evaluationPanel0, 0, wxALL, 5 );

    evaluationPanel1 = new HCUBE::EvaluationPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 128,128 ), wxTAB_TRAVERSAL );
    evaluationPanel1->SetMinSize( wxSize( 128,128 ) );

    evaluationSizer->Add( evaluationPanel1, 0, wxALL, 5 );

    evaluationPanel2 = new HCUBE::EvaluationPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 128,128 ), wxTAB_TRAVERSAL );
    evaluationPanel2->SetMinSize( wxSize( 128,128 ) );

    evaluationSizer->Add( evaluationPanel2, 0, wxALL, 5 );

    evaluationPanel3 = new HCUBE::EvaluationPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 128,128 ), wxTAB_TRAVERSAL );
    evaluationPanel3->SetMinSize( wxSize( 128,128 ) );

    evaluationSizer->Add( evaluationPanel3, 0, wxALL, 5 );

    evaluationPanel4 = new HCUBE::EvaluationPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 128,128 ), wxTAB_TRAVERSAL );
    evaluationPanel4->SetMinSize( wxSize( 128,128 ) );

    evaluationSizer->Add( evaluationPanel4, 0, wxALL, 5 );

    evaluationPanel5 = new HCUBE::EvaluationPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 128,128 ), wxTAB_TRAVERSAL );
    evaluationPanel5->SetMinSize( wxSize( 128,128 ) );

    evaluationSizer->Add( evaluationPanel5, 0, wxALL, 5 );

    evaluationPanel6 = new HCUBE::EvaluationPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 128,128 ), wxTAB_TRAVERSAL );
    evaluationPanel6->SetMinSize( wxSize( 128,128 ) );

    evaluationSizer->Add( evaluationPanel6, 0, wxALL, 5 );

    evaluationPanel7 = new HCUBE::EvaluationPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 128,128 ), wxTAB_TRAVERSAL );
    evaluationPanel7->SetMinSize( wxSize( 128,128 ) );

    evaluationSizer->Add( evaluationPanel7, 0, wxALL, 5 );

    rootSizer->Add( evaluationSizer, 0, 0, 5 );

    m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
    highResPanel = new HCUBE::EvaluationPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    m_notebook1->AddPage( highResPanel, wxT("High-Res Image"), true );
    networkPanel = new HCUBE::NeuralNetworkPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    m_notebook1->AddPage( networkPanel, wxT("Network"), false );
    votingPanel = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizer51;
    bSizer51 = new wxBoxSizer( wxVERTICAL );

    wxStaticBoxSizer* sbSizer2;
    sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( votingPanel, -1, wxT("Votes") ), wxVERTICAL );

    wxGridSizer* gSizer3;
    gSizer3 = new wxGridSizer( 4, 2, 0, 0 );

    voteButton0 = new wxToggleButton( votingPanel, wxID_ANY, wxT("Vote"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer3->Add( voteButton0, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );

    voteButton1 = new wxToggleButton( votingPanel, wxID_ANY, wxT("Vote"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer3->Add( voteButton1, 0, wxALL, 5 );

    voteButton2 = new wxToggleButton( votingPanel, wxID_ANY, wxT("Vote"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer3->Add( voteButton2, 0, wxALL, 5 );

    voteButton3 = new wxToggleButton( votingPanel, wxID_ANY, wxT("Vote"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer3->Add( voteButton3, 0, wxALL, 5 );

    voteButton4 = new wxToggleButton( votingPanel, wxID_ANY, wxT("Vote"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer3->Add( voteButton4, 0, wxALL, 5 );

    voteButton5 = new wxToggleButton( votingPanel, wxID_ANY, wxT("Vote"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer3->Add( voteButton5, 0, wxALL, 5 );

    voteButton6 = new wxToggleButton( votingPanel, wxID_ANY, wxT("Vote"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer3->Add( voteButton6, 0, wxALL, 5 );

    voteButton7 = new wxToggleButton( votingPanel, wxID_ANY, wxT("Vote"), wxDefaultPosition, wxDefaultSize, 0 );
    gSizer3->Add( voteButton7, 0, wxALL, 5 );

    sbSizer2->Add( gSizer3, 0, 0, 5 );

    bSizer51->Add( sbSizer2, 0, 0, 5 );

    wxBoxSizer* bSizer7;
    bSizer7 = new wxBoxSizer( wxHORIZONTAL );

    advanceGenerationButton = new wxButton( votingPanel, wxID_ANY, wxT("Advance Generation"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer7->Add( advanceGenerationButton, 0, wxALL, 5 );

    resetVotesButton = new wxButton( votingPanel, wxID_ANY, wxT("Reset votes"), wxDefaultPosition, wxDefaultSize, 0 );
    bSizer7->Add( resetVotesButton, 0, wxALL, 5 );

    bSizer51->Add( bSizer7, 1, wxEXPAND, 5 );

    votingPanel->SetSizer( bSizer51 );
    votingPanel->Layout();
    bSizer51->Fit( votingPanel );
    m_notebook1->AddPage( votingPanel, wxT("Voting"), false );

    rootSizer->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );

    this->SetSizer( rootSizer );
    this->Layout();
}

NeuralNetworkPanelTemplate::NeuralNetworkPanelTemplate( wxWindow* parent, int id, wxPoint pos, wxSize size, int style ) : wxPanel( parent, id, pos, size, style )
{
    this->SetMinSize( wxSize( 512,512 ) );

    wxBoxSizer* bSizer4;
    bSizer4 = new wxBoxSizer( wxVERTICAL );

    networkVisualizationPanel = new HCUBE::NetworkPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 512,200 ), wxTAB_TRAVERSAL );
    networkVisualizationPanel->SetMinSize( wxSize( 512,200 ) );

    bSizer4->Add( networkVisualizationPanel, 3, wxEXPAND | wxALL, 5 );

    networkAdjacency = new HCUBE::NeuralNetworkWeightGrid( this, wxID_ANY, wxDefaultPosition, wxSize( 512,150 ), wxHSCROLL|wxVSCROLL );
    networkAdjacency->CreateGrid( 5, 5 );
    bSizer4->Add( networkAdjacency, 1, wxALL|wxEXPAND, 5 );

    this->SetSizer( bSizer4 );
    this->Layout();
}

ViewIndividualFrameTemplate::ViewIndividualFrameTemplate( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxFrame( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer5;
    bSizer5 = new wxBoxSizer( wxVERTICAL );

    m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
    m_notebook2->SetMaxSize( wxSize( 1024,768 ) );

    evaluationPanel = new HCUBE::EvaluationPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxSize( 640,480 ), wxTAB_TRAVERSAL );
    m_notebook2->AddPage( evaluationPanel, wxT("Evaluation"), false );
    networkPanel = new HCUBE::NeuralNetworkPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxSize( 640,480 ), wxTAB_TRAVERSAL );
    m_notebook2->AddPage( networkPanel, wxT("Network"), false );

    bSizer5->Add( m_notebook2, 1, wxALL|wxEXPAND, 0 );

    this->SetSizer( bSizer5 );
    this->Layout();
}

TicTacToeGameExperimentPanelTemplate::TicTacToeGameExperimentPanelTemplate( wxWindow* parent, int id, wxPoint pos, wxSize size, int style ) : wxPanel( parent, id, pos, size, style )
{}

#endif

