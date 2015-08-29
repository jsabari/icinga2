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

#ifndef API_H
#define API_H

#include "base/string.hpp"
#include <vector>

namespace icinga
{

struct ApiFieldAttributes
{
public:
	bool Config;
	bool Internal;
	bool Required;
	bool State;
};

class ApiType;

struct ApiField
{
public:
	String Name;
	int ID;
	int ArrayRank;
	ApiFieldAttributes FieldAttributes;
	String TypeName;
	intrusive_ptr<ApiType> Type;
};

class ApiType : public Object
{
public:
	DECLARE_PTR_TYPEDEFS(ApiType);

	String Name;
	String BaseName;
	ApiType::Ptr Base;
	bool Abstract;
	std::map<String, ApiField> Fields;
	std::vector<String> PrototypeKeys;
};

struct ApiObjectReference
{
public:
	String Name;
	String Type;
};

struct ApiObject
{
public:
	std::map<String, Value> Attrs;
	std::vector<ApiObjectReference> UsedBy;
};

class ApiClient : public Object
{
public:
	DECLARE_PTR_TYPEDEFS(ApiClient);

	ApiClient(const String& host, unsigned short port,
	    const String& user, const String& password);

	std::vector<ApiType> GetTypes(void) const;
	std::vector<ApiObject> GetObjects(const String& type,
	    const std::vector<String>& names = std::vector<String>(),
	    const std::vector<String>& attrs = std::vector<String>()) const;

private:
	static String GetPluralTypeName(const String& type);
};

}

#endif /* API_H */