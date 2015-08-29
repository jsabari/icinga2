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

#include "icinga-studio/connectform.hpp"
#include "icinga-studio/mainform.hpp"
#include <wx/wx.h>
#include <wx/app.h>
#include <wx/config.h>

using namespace icinga;

class IcingaStudio : public wxApp
{
public:
	virtual bool OnInit(void) override
	{
		m_Config = new wxConfig("IcingaStudio");

		wxString wurl;

		if (!m_Config->Read("url", &wurl))
			wurl = "https://localhost:5665/";

		std::string url = wurl.ToStdString();

		ConnectForm f(NULL, new Url(url));
		if (f.ShowModal() != wxID_OK)
			return false;

		url = f.GetUrl()->Format();
		wurl = url;
		m_Config->Write("url", wurl);

		MainForm *m = new MainForm(NULL);
		m->Show();

		return true;
	}

private:
	wxConfig *m_Config;
};

wxIMPLEMENT_APP(IcingaStudio);
