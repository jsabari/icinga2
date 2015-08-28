///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __FORMS_H__
#define __FORMS_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/treectrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainFormBase
///////////////////////////////////////////////////////////////////////////////
class MainFormBase : public wxFrame 
{
	private:
	
	protected:
		wxTreeCtrl* m_treeCtrl1;
		wxListCtrl* m_listCtrl1;
		wxListCtrl* m_listCtrl2;
	
	public:
		
		MainFormBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 667,487 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFormBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ConnectFormBase
///////////////////////////////////////////////////////////////////////////////
class ConnectFormBase : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_ConnectionDetailsPanel;
		wxStaticText* m_HostLabel;
		wxTextCtrl* m_HostText;
		wxStaticText* m_PortLabel;
		wxTextCtrl* m_PortText;
		wxStaticText* m_UserLabel;
		wxTextCtrl* m_UserText;
		wxStaticText* m_PasswordLabel;
		wxTextCtrl* m_PasswordText;
		wxStaticText* m_InfoLabel;
		wxButton* m_OKButton;
		wxButton* m_CancelButton;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnOKClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancelClicked( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ConnectFormBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Icinga Studio - Connect"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 336,388 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~ConnectFormBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class AboutForm
///////////////////////////////////////////////////////////////////////////////
class AboutForm : public wxFrame 
{
	private:
	
	protected:
	
	public:
		
		AboutForm( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~AboutForm();
	
};

#endif //__FORMS_H__
