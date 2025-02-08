#include "glib/reflect/FieldAccessor.h"
#include "glib/glibError.h"
#include "glib/graphics/Sprite.h"

#include <iostream>
#include <regex>
#include <cstring>

using namespace glib;

glib::FieldAccessor::FieldAccessor()
{
}

glib::FieldAccessor::~FieldAccessor()
{
}

void glib::FieldAccessor::RegisterField(const Field& field)
{
	____fields.insert({ field.name, field });
}

std::optional<const Field> glib::FieldAccessor::GetField(const std::string& name)
{
	return _GetField(____fields, name);
}

bool glib::FieldAccessor::ModifyField(const std::string& name, float value)
{
	return _ModifyField(name, FieldType::FLOAT, &value);
}

bool glib::FieldAccessor::ModifyField(const std::string& name, uint32_t value)
{
	return _ModifyField(name, FieldType::INT32, &value);
}

bool glib::FieldAccessor::ModifyField(const std::string& name, const Vec2& value)
{
	return _ModifyField(name, FieldType::VEC2, (void*)&value);
}

bool glib::FieldAccessor::ModifyField(const std::string& name, const Vec3& value)
{
	return _ModifyField(name, FieldType::VEC3, (void*)&value);
}

bool glib::FieldAccessor::ModifyField(const std::string& name, const Vec4& value)
{
	return _ModifyField(name, FieldType::VEC4, (void*)&value);
}

bool glib::FieldAccessor::ModifyField(const std::string& name, const Color& value)
{
	return _ModifyField(name, FieldType::COLOR, (void*)&value);
}

bool glib::FieldAccessor::ModifyField(const std::string& name, bool value)
{
	return _ModifyField(name, FieldType::BOOL, (void*)&value);
}

bool glib::FieldAccessor::ModifyField(const std::string& name, void* value)
{
	return _ModifyField(name, FieldType::PTR, value);
}

std::optional<Field> glib::FieldAccessor::_GetField(std::unordered_map<std::string, Field>& fields, const std::string& name)
{
	std::regex re("/");
	std::sregex_token_iterator it(name.begin(), name.end(), re, -1);
	std::sregex_token_iterator end;
	std::vector<std::string> accessors(it, end);
	if (accessors.size() < 1) return std::nullopt;

	std::string nextPath = "";
	for (int i = 1; i < accessors.size(); i++)
	{
		nextPath += accessors.at(i);
	}

	if (!fields.contains(accessors.at(0))) return std::nullopt;
	Field& f = fields.at(accessors.at(0));
	if (f.type == FieldType::MODIFIABLE)
	{
		if (accessors.size() >= 2)
		{
			void* advancedPtr = (void*)((size_t)f.value + f.o);
			return _GetField(*((std::unordered_map<std::string, Field>*)advancedPtr), nextPath);
		}
		else
		{
			Error::Throw(GLIB_REFLECT_INVALID_TYPE, "Invalid path for MODIFIABLE: " + name);
			return std::nullopt;
		}
	}
	else
	{
		if (accessors.size() > 1)
		{
			Error::Throw(GLIB_REFLECT_INVALID_TYPE, "Invalid path: " + name);
			return std::nullopt;
		}
		return f;
	}
}

bool glib::FieldAccessor::_ModifyField(const std::string& name, FieldType type, void* newValue)
{
	std::optional<Field> f = _GetField(____fields, name);
	if (!f.has_value())
	{
		Error::Throw(GLIB_REFLECT_NO_SUCH_FIELD, "Failed to modify field: " + name);
		return false;
	}

	Field& field = f.value();
	if (field.type != type)
	{
		Error::Throw(GLIB_REFLECT_INVALID_TYPE, "Tried to modify field with different type (" + name + ")");
		return false;
	}

	memcpy(field.value, newValue, field.size);

	return true;
}
