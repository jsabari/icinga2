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

#include "remote/actionshandler.hpp"
#include "remote/httputility.hpp"
#include "remote/filterutility.hpp"
#include "remote/apiaction.hpp"
#include "base/exception.hpp"
#include "base/serializer.hpp"
#include "base/logger.hpp"
#include <boost/algorithm/string.hpp>
#include <set>

using namespace icinga;

REGISTER_URLHANDLER("/v1/actions", ActionsHandler);

bool ActionsHandler::HandleRequest(const ApiUser::Ptr& user, HttpRequest& request, HttpResponse& response)
{
	if (request.RequestUrl->GetPath().size() < 3)
		return false;

	Dictionary::Ptr result = new Dictionary();

	if (request.RequestMethod != "POST") {
		response.SetStatus(400, "Bad request");
		result->Set("error", "Bad request, must be POST");
		HttpUtility::SendJsonBody(response, result);
		return true;
	}

	String actionName = request.RequestUrl->GetPath()[2];

	ApiAction::Ptr action = ApiAction::GetByName(actionName);

	if (!action) {
		response.SetStatus(404, "Action not found");
		result->Set("error", "No action '" + actionName + "' exist");
		HttpUtility::SendJsonBody(response, result);
		return true;
	}

	QueryDescription qd;

	Dictionary::Ptr params = HttpUtility::FetchRequestParameters(request);

	const std::vector<String>& types = action->GetTypes();
	std::vector<Value> objs;

	if (!types.empty()) {
		qd.Types = std::set<String>(types.begin(), types.end());

		try {
			objs = FilterUtility::GetFilterTargets(qd, params);
		} catch (const std::invalid_argument& ex) {
			response.SetStatus(400, "Invalid type-filter combination");
			result->Set("error", DiagnosticInformation(ex, false));
			HttpUtility::SendJsonBody(response, result);
			return true;
		}

	} else
		objs.push_back(ConfigObject::Ptr());

	Array::Ptr results = new Array();

	Log(LogNotice, "ApiActionHandler")
	    << "Running action " << actionName;

	BOOST_FOREACH(const ConfigObject::Ptr& obj, objs) {
		try {
			results->Add(action->Invoke(obj, params));
		} catch (const std::exception& ex) {
			Dictionary::Ptr fail = new Dictionary();
			fail->Set("code", 501);
			fail->Set("status", "Error: " + DiagnosticInformation(ex, false));
			results->Add(fail);
		}
	}

	result->Set("results", results);

	response.SetStatus(200, "OK");
	HttpUtility::SendJsonBody(response, result);

	return true;
}

