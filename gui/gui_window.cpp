// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "gui_window.hpp"

guiFrame::guiFrame( const wxString& title, const wxPoint& pos, const wxSize& size)
  : wxFrame( NULL, wxID_ANY, title, pos, size )
{
  wxMenu *menuOne = new wxMenu;
  menuOne->Append( ID_Generate, "&Generate\tCtrl-G", "Generate a new password" );
  menuOne->Append( ID_AlgoList, "List &Algorithms\tCtrl-A", "List all available generating algorithms" );
  menuOne->Append( ID_Configure, "C&onfigure\tCtrl-O", "Configure the generating parameters" );
  menuOne->AppendSeparator();
  menuOne->Append(wxID_EXIT);

  wxMenu *menuTwo = new wxMenu;
  menuTwo->Append( wxID_ABOUT );

  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append( menuOne, "&File" );
  menuBar->Append( menuTwo, "&Help" );

  SetMenuBar( menuBar );
  CreateStatusBar();
  SetStatusText( "Everything seems to be working" );
}

void guiFrame::OnExit(wxCommandEvent& ev) {
  Close( true );
}

void guiFrame::OnAbout(wxCommandEvent& ev) {
  wxMessageBox( "This is the GUI front-end to ShadowWolf's Password Generator",
		"About passgen-gui", wxOK | wxICON_INFORMATION );
}

void guiFrame::OnGenerate(wxCommandEvent& ev) {
// grab selected algo name from configuration store
// grab selected password length from configuration store
// get pointer to selected algo from plugin store
// generate password
// pop message box with password in it
}

void guiFrame::OnConfigure(wxCommandEvent& ev) {
// pop dialog with radio buttons for configuration of algo
// have spinner for selecting password length
// add options to configuration store for specifying additional possible controls
// so plugins with additional options can add them
}

void guiFrame::OnAlgoList(wxCommandEvent& ev) {
// get list of all loaded plugins
// get list of provided algos from plugins
// display message box with information nicely formatted
}
