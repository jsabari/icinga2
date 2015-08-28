///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "forms.h"

///////////////////////////////////////////////////////////////////////////

MainFormBase::MainFormBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_treeCtrl1 = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer8->Add( m_treeCtrl1, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_listCtrl1 = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	bSizer9->Add( m_listCtrl1, 0, wxALL|wxEXPAND, 5 );
	
	m_listCtrl2 = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	bSizer9->Add( m_listCtrl2, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer8->Add( bSizer9, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer8 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

MainFormBase::~MainFormBase()
{
}

ConnectFormBase::ConnectFormBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* m_DialogSizer;
	m_DialogSizer = new wxBoxSizer( wxVERTICAL );
	
	m_ConnectionDetailsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* m_DetailsSizer;
	m_DetailsSizer = new wxStaticBoxSizer( new wxStaticBox( m_ConnectionDetailsPanel, wxID_ANY, wxT("Connection Details") ), wxVERTICAL );
	
	m_HostLabel = new wxStaticText( m_DetailsSizer->GetStaticBox(), wxID_ANY, wxT("Host:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_HostLabel->Wrap( -1 );
	m_DetailsSizer->Add( m_HostLabel, 0, wxALL, 5 );
	
	m_HostText = new wxTextCtrl( m_DetailsSizer->GetStaticBox(), wxID_OK, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_DetailsSizer->Add( m_HostText, 0, wxALL|wxEXPAND, 5 );
	
	m_PortLabel = new wxStaticText( m_DetailsSizer->GetStaticBox(), wxID_ANY, wxT("Port:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PortLabel->Wrap( -1 );
	m_DetailsSizer->Add( m_PortLabel, 0, wxALL, 5 );
	
	m_PortText = new wxTextCtrl( m_DetailsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_DetailsSizer->Add( m_PortText, 0, wxALL, 5 );
	
	m_UserLabel = new wxStaticText( m_DetailsSizer->GetStaticBox(), wxID_ANY, wxT("API User:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_UserLabel->Wrap( -1 );
	m_DetailsSizer->Add( m_UserLabel, 0, wxALL, 5 );
	
	m_UserText = new wxTextCtrl( m_DetailsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_DetailsSizer->Add( m_UserText, 0, wxALL|wxEXPAND, 5 );
	
	m_PasswordLabel = new wxStaticText( m_DetailsSizer->GetStaticBox(), wxID_ANY, wxT("API Password:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PasswordLabel->Wrap( -1 );
	m_DetailsSizer->Add( m_PasswordLabel, 0, wxALL, 5 );
	
	m_PasswordText = new wxTextCtrl( m_DetailsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	m_DetailsSizer->Add( m_PasswordText, 0, wxALL|wxEXPAND, 5 );
	
	m_InfoLabel = new wxStaticText( m_DetailsSizer->GetStaticBox(), wxID_ANY, wxT("You can find the username and password for the default user in /etc/icinga2/conf.d/api-users.conf."), wxDefaultPosition, wxDefaultSize, 0 );
	m_InfoLabel->Wrap( 270 );
	m_DetailsSizer->Add( m_InfoLabel, 0, wxALL, 5 );
	
	
	m_ConnectionDetailsPanel->SetSizer( m_DetailsSizer );
	m_ConnectionDetailsPanel->Layout();
	m_DetailsSizer->Fit( m_ConnectionDetailsPanel );
	m_DialogSizer->Add( m_ConnectionDetailsPanel, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* m_ButtonsSizer;
	m_ButtonsSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_OKButton = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ButtonsSizer->Add( m_OKButton, 0, wxALL, 5 );
	
	m_CancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ButtonsSizer->Add( m_CancelButton, 0, wxALL, 5 );
	
	
	m_DialogSizer->Add( m_ButtonsSizer, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( m_DialogSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_OKButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConnectFormBase::OnOKClicked ), NULL, this );
	m_CancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConnectFormBase::OnCancelClicked ), NULL, this );
}

ConnectFormBase::~ConnectFormBase()
{
	// Disconnect Events
	m_OKButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConnectFormBase::OnOKClicked ), NULL, this );
	m_CancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConnectFormBase::OnCancelClicked ), NULL, this );
	
}

AboutForm::AboutForm( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	
	this->Centre( wxBOTH );
}

AboutForm::~AboutForm()
{
}
