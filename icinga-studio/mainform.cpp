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
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/foreach.hpp>

using namespace icinga;

MainForm::MainForm(wxWindow *parent, const Url::Ptr& url)
	: MainFormBase(parent)
{
#ifdef _WIN32
	SetIcon(wxICON(icinga));
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
#endif /* _WIN32 */

	String host, port, user, pass;

	std::string authority = url->GetAuthority();

	std::vector<std::string> tokens;
	boost::algorithm::split(tokens, authority, boost::is_any_of("@"));

	if (tokens.size() > 1) {
		std::vector<std::string> userinfo;
		boost::algorithm::split(userinfo, tokens[0], boost::is_any_of(":"));

		user = userinfo[0];
		pass = userinfo[1];
	}

	std::vector<std::string> hostport;
	boost::algorithm::split(hostport, tokens.size() > 1 ? tokens[1] : tokens[0], boost::is_any_of(":"));

	host = hostport[0];

	if (hostport.size() > 1)
		port = hostport[1];
	else
		port = "5665";

	m_ApiClient = new ApiClient(host, port, user, pass);
	m_ApiClient->GetTypes(boost::bind(&MainForm::TypesCompletionHandler, this, _1, true));

	std::string title = host;
	
	if (port != "5665")
		title += +":" + port;
	
	title += " - Icinga Studio";
	SetTitle(title);

	m_ObjectsList->InsertColumn(0, "Name", 0, 300);

	m_PropertiesList->InsertColumn(0, "Name", 0, 250);
	m_PropertiesList->InsertColumn(1, "Value", 0, 300);

	m_TypesRoot = m_TypesTree->AddRoot("root");
}

void MainForm::TypesCompletionHandler(const std::vector<ApiType::Ptr>& types, bool forward)
{
	if (forward) {
		CallAfter(boost::bind(&MainForm::TypesCompletionHandler, this, types, false));
		return;
	}

	bool all = false;
	std::map<String, wxTreeItemId> items;

	while (!all) {
		all = true;

		BOOST_FOREACH(const ApiType::Ptr& type, types) {
			std::string name = type->Name;

			if (items.find(name) != items.end())
				continue;

			all = false;

			wxTreeItemId parent;
			
			if (type->BaseName.IsEmpty())
				parent = m_TypesRoot;
			else {
				std::map<String, wxTreeItemId>::const_iterator it = items.find(type->BaseName);

				if (it == items.end())
					continue;

				parent = it->second;
			}

			items[name] = m_TypesTree->AppendItem(parent, name, 0);
		}
	}
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
