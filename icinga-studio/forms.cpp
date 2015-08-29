///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "forms.h"

#include "icinga.xpm"

///////////////////////////////////////////////////////////////////////////

MainFormBase::MainFormBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_MenuBar = new wxMenuBar( 0 );
	m_FileMenu = new wxMenu();
	wxMenuItem* m_ConnectMenuItem;
	m_ConnectMenuItem = new wxMenuItem( m_FileMenu, ID_CONNECT, wxString( wxT("&Connect") ) , wxEmptyString, wxITEM_NORMAL );
	m_FileMenu->Append( m_ConnectMenuItem );
	
	wxMenuItem* m_QuitMenuItem;
	m_QuitMenuItem = new wxMenuItem( m_FileMenu, ID_QUIT, wxString( wxT("&Quit") ) , wxEmptyString, wxITEM_NORMAL );
	m_FileMenu->Append( m_QuitMenuItem );
	
	m_MenuBar->Append( m_FileMenu, wxT("&File") ); 
	
	m_HelpMenu = new wxMenu();
	wxMenuItem* m_AboutMenuItem;
	m_AboutMenuItem = new wxMenuItem( m_HelpMenu, ID_ABOUT, wxString( wxT("&About") ) , wxEmptyString, wxITEM_NORMAL );
	m_HelpMenu->Append( m_AboutMenuItem );
	
	m_MenuBar->Append( m_HelpMenu, wxT("&Help") ); 
	
	this->SetMenuBar( m_MenuBar );
	
	wxBoxSizer* m_DialogSizer;
	m_DialogSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_ConnectionDetailsSizer;
	m_ConnectionDetailsSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_TypesTree = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 315,-1 ), wxTR_DEFAULT_STYLE );
	m_ConnectionDetailsSizer->Add( m_TypesTree, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* m_ObjectDetailsSizer;
	m_ObjectDetailsSizer = new wxBoxSizer( wxVERTICAL );
	
	m_ObjectsList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	m_ObjectDetailsSizer->Add( m_ObjectsList, 1, wxALL|wxEXPAND, 5 );
	
	m_PropertiesList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	m_ObjectDetailsSizer->Add( m_PropertiesList, 1, wxALL|wxEXPAND, 5 );
	
	
	m_ConnectionDetailsSizer->Add( m_ObjectDetailsSizer, 1, wxEXPAND, 5 );
	
	
	m_DialogSizer->Add( m_ConnectionDetailsSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_DialogSizer );
	this->Layout();
	m_StatusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( m_QuitMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFormBase::OnQuitClicked ) );
	this->Connect( m_AboutMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFormBase::OnAboutClicked ) );
}

MainFormBase::~MainFormBase()
{
	// Disconnect Events
	this->Disconnect( ID_QUIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFormBase::OnQuitClicked ) );
	this->Disconnect( ID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFormBase::OnAboutClicked ) );
	
}

ConnectFormBase::ConnectFormBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* m_DialogSizer;
	m_DialogSizer = new wxBoxSizer( wxVERTICAL );
	
	wxPanel* m_ConnectionDetailsPanel;
	m_ConnectionDetailsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* m_DetailsSizer;
	m_DetailsSizer = new wxStaticBoxSizer( new wxStaticBox( m_ConnectionDetailsPanel, wxID_ANY, wxT("Connection Details") ), wxVERTICAL );
	
	wxStaticText* m_HostLabel;
	m_HostLabel = new wxStaticText( m_DetailsSizer->GetStaticBox(), wxID_ANY, wxT("Host:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_HostLabel->Wrap( -1 );
	m_DetailsSizer->Add( m_HostLabel, 0, wxALL, 5 );
	
	m_HostText = new wxTextCtrl( m_DetailsSizer->GetStaticBox(), wxID_OK, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_DetailsSizer->Add( m_HostText, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_PortLabel;
	m_PortLabel = new wxStaticText( m_DetailsSizer->GetStaticBox(), wxID_ANY, wxT("Port:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PortLabel->Wrap( -1 );
	m_DetailsSizer->Add( m_PortLabel, 0, wxALL, 5 );
	
	m_PortText = new wxTextCtrl( m_DetailsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_DetailsSizer->Add( m_PortText, 0, wxALL, 5 );
	
	wxStaticText* m_UserLabel;
	m_UserLabel = new wxStaticText( m_DetailsSizer->GetStaticBox(), wxID_ANY, wxT("API User:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_UserLabel->Wrap( -1 );
	m_DetailsSizer->Add( m_UserLabel, 0, wxALL, 5 );
	
	m_UserText = new wxTextCtrl( m_DetailsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_DetailsSizer->Add( m_UserText, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_PasswordLabel;
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
	
	m_OKButton = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ButtonsSizer->Add( m_OKButton, 0, wxALL, 5 );
	
	m_CancelButton = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ButtonsSizer->Add( m_CancelButton, 0, wxALL, 5 );
	
	
	m_DialogSizer->Add( m_ButtonsSizer, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( m_DialogSizer );
	this->Layout();
	m_DialogSizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_InfoLabel->Connect( wxEVT_SIZE, wxSizeEventHandler( ConnectFormBase::OnResizeInfoLabel ), NULL, this );
	m_OKButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConnectFormBase::OnOKClicked ), NULL, this );
	m_CancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConnectFormBase::OnCancelClicked ), NULL, this );
}

ConnectFormBase::~ConnectFormBase()
{
	// Disconnect Events
	m_InfoLabel->Disconnect( wxEVT_SIZE, wxSizeEventHandler( ConnectFormBase::OnResizeInfoLabel ), NULL, this );
	m_OKButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConnectFormBase::OnOKClicked ), NULL, this );
	m_CancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConnectFormBase::OnCancelClicked ), NULL, this );
	
}

AboutFormBase::AboutFormBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBitmap* m_ProductIcon;
	m_ProductIcon = new wxStaticBitmap( this, wxID_ANY, wxBitmap( icinga_xpm ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_ProductIcon, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_ProductNameLabel;
	m_ProductNameLabel = new wxStaticText( this, wxID_ANY, wxT("Icinga Studio"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProductNameLabel->Wrap( -1 );
	bSizer14->Add( m_ProductNameLabel, 0, wxALL, 5 );
	
	m_VersionLabel = new wxStaticText( this, wxID_ANY, wxT("Version"), wxDefaultPosition, wxDefaultSize, 0 );
	m_VersionLabel->Wrap( -1 );
	bSizer14->Add( m_VersionLabel, 0, wxALL, 5 );
	
	wxStaticText* m_CopyrightLabel;
	m_CopyrightLabel = new wxStaticText( this, wxID_ANY, wxT("Copyright (c) 2015 Icinga Development Team"), wxDefaultPosition, wxDefaultSize, 0 );
	m_CopyrightLabel->Wrap( -1 );
	bSizer14->Add( m_CopyrightLabel, 0, wxALL, 5 );
	
	wxButton* m_OKButton;
	m_OKButton = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_OKButton, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	bSizer13->Add( bSizer14, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer13 );
	this->Layout();
	bSizer13->Fit( this );
	
	this->Centre( wxBOTH );
}

AboutFormBase::~AboutFormBase()
{
}
