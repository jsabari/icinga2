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

#include "icinga-studio/api.hpp"
#include "remote/base64.hpp"
#include "base/json.hpp"
#include <boost/foreach.hpp>

using namespace icinga;

ApiClient::ApiClient(const String& host, const String& port,
    const String& user, const String& password)
    : m_Connection(new HttpClientConnection(host, port, true)), m_User(user), m_Password(password)
{
	m_Connection->Start();
}

void ApiClient::GetTypes(const TypesCompletionCallback& callback) const
{
	boost::shared_ptr<HttpRequest> req = m_Connection->NewRequest();
	req->RequestMethod = "GET";
	req->RequestUrl = new Url("https://" + m_Connection->GetHost() + ":" + m_Connection->GetPort() + "/v1/types");
	req->AddHeader("Authorization", "Basic " + Base64::Encode(m_User + ":" + m_Password));
	m_Connection->SubmitRequest(req, boost::bind(TypesHttpCompletionCallback, _1, _2, callback));
}

void ApiClient::TypesHttpCompletionCallback(HttpRequest& request, HttpResponse& response,
    const TypesCompletionCallback& callback)
{
	Dictionary::Ptr result;

	String body;
	char buffer[1024];
	size_t count;

	while ((count = response.ReadBody(buffer, sizeof(buffer))) > 0)
		body += String(buffer, buffer + count);

	if (!body.IsEmpty())
		result = JsonDecode(body);

	Array::Ptr results = result->Get("results");

	std::vector<ApiType::Ptr> types;

	ObjectLock olock(results);
	BOOST_FOREACH(const Dictionary::Ptr typeInfo, results) {
		ApiType::Ptr type = new ApiType();;
		type->Abstract = typeInfo->Get("abstract");
		type->BaseName = typeInfo->Get("base");
		type->Name = typeInfo->Get("name");
		type->PluralName = typeInfo->Get("plural_name");
		types.push_back(type);
	}

	callback(types);
}

std::vector<ApiObject> ApiClient::GetObjects(const String& type,
    const std::vector<String>& names, const std::vector<String>& attrs) const
{
	return std::vector<ApiObject>();
}
