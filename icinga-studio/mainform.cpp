/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012-2015 Icinga Development Team (http://www.icinga.org)    *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#include "icinga-studio/mainform.hpp"
#include "icinga-studio/aboutform.hpp"

using namespace icinga;

MainForm::MainForm(wxWindow *parent, const Url::Ptr& url)
	: MainFormBase(parent)
{
#ifdef _WIN32
	SetIcon(wxICON(icinga));
#endif /* _WIN32 */
	
#ifdef __WXOSX__
	m_MenuBar->Remove(0);
	m_MenuBar->Remove(0);
	m_MenuBar->Append(m_QuitMenuItem);
	m_MenuBar->Append(m_AboutMenuItem);
#endif /* __WXOSX__ */

	std::string sUrl = url->Format();
	SetTitle(sUrl + " - Icinga Studio");

	m_ApiClient = new ApiClient("mon.shroudbox.net", 5665, "root", "U1aIqWl4UnQP");

	m_ObjectsList->InsertColumn(0, "Name", 0, 300);

	m_PropertiesList->InsertColumn(0, "Name", 0, 250);
	m_PropertiesList->InsertColumn(1, "Value", 0, 300);

	wxTreeItemId id = m_TypesTree->AddRoot("Object", 0);
	m_TypesTree->AppendItem(id, "ConfigObject", 0);
}

void MainForm::OnQuitClicked(wxCommandEvent& event)
{
	Close();
}


void MainForm::OnAboutClicked(wxCommandEvent& event)
{
	AboutForm form(this);
	form.ShowModal();
}
