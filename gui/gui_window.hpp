#ifndef __GUI_FRAME_HPP__
#define __GUI_FRAME_HPP__
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class guiFrame: public wxFrame {
  public:
    //events
    virtual bool OnAbout(wxCommandEvent& ev);
    virtual bool OnExit(wxCommandEvent& ev);
    virtual bool OnHelp(wxCommandEvent& ev);
    virtual bool OnGenerate(wxCommandEvent& ev);
    virtual bool OnAlgoList(wxCommandEvent& ev);
    virtual bool OnConfigure(wxCommandEvent& ev);

    wxDECLARE_EVENT_TABLE();
};

enum {
  ID_Generate = 1,
  ID_AlgoList,
  ID_Configure
};

wxBEGIN_EVENT_TABLE(guiFrame, wxFrame)
EVT_MENU( ID_Generate, guiFrame::OnGenerate )
EVT_MENU( ID_AlgoList, guiFrame::OnAlgoList )
EVT_MENU( ID_Configure, guiFrame::OnConfigure )
EVT_MENU( wxID_EXIT, guiFrame::OnExit )
EVT_MENU( wxID_ABOUT, guiFrame::OnAbout )
wxEND_EVENT_TABLE()

#endif
