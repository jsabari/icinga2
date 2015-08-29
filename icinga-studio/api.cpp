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

using namespace icinga;

ApiClient::ApiClient(const String& host, unsigned short port,
    const String& user, const String& password)
{

}

std::vector<ApiType> ApiClient::GetTypes(void) const
{
	return std::vector<ApiType>();
}

std::vector<ApiObject> ApiClient::GetObjects(const String& type,
    const std::vector<String>& names, const std::vector<String>& attrs) const
{
	return std::vector<ApiObject>();
}

String ApiClient::GetPluralTypeName(const String& type)
{
	if (type[type.GetLength() - 1] == 'y')
		return type.SubStr(0, type.GetLength() - 1) + "ies";
	else
		return type + "s";
}