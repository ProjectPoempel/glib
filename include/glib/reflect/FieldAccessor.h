#pragma once

#include "../DLLDefs.h"
#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../utils/Color.h"

#include <string>
#include <unordered_map>
#include <optional>
#include <cstdint>

#define CLASS_NAME() ("class " + (std::string)typeid(*this).name())
#define GLIB_GET_FIELD_NAME(field) #field
#define GLIB_FIELD_STRUCT(field, type) { CLASS_NAME() + "::" + #field, #field, &field, sizeof(field), glib::FieldType::type }
#define GLIB_FIELD_STRUCT_MOD(field, type, cls) { CLASS_NAME() + "::" + #field, #field, &field, sizeof(field), glib::FieldType::type, offsetof(cls, ____fields) }

namespace glib
{
	GLIB_API enum class FieldType
	{
		FLOAT,
		INT32,
		VEC2,
		VEC3,
		VEC4,
		COLOR,
		BOOL,
		PTR,
		MODIFIABLE
	};

	struct Field
	{
		std::string name;
		std::string simpleName;
		void* value;
		size_t size;
		FieldType type;
		size_t o;
	};

	class FieldAccessor
	{
	public:
		std::unordered_map<std::string, Field> ____fields;
	public:
		GLIB_API FieldAccessor();
		GLIB_API ~FieldAccessor();
	protected:
		GLIB_API void RegisterField(const Field& field);
	public:
		GLIB_API std::optional<const Field> GetField(const std::string& name);
		GLIB_API bool ModifyField(const std::string& name, float value);
		GLIB_API bool ModifyField(const std::string& name, uint32_t value);
		GLIB_API bool ModifyField(const std::string& name, const Vec2& value);
		GLIB_API bool ModifyField(const std::string& name, const Vec3& value);
		GLIB_API bool ModifyField(const std::string& name, const Vec4& value);
		GLIB_API bool ModifyField(const std::string& name, const Color& value);
		GLIB_API bool ModifyField(const std::string& name, bool value);
		GLIB_API bool ModifyField(const std::string& name, void* value);
	private:
		std::optional<Field> _GetField(std::unordered_map<std::string, Field>& fields, const std::string& name);
		bool _ModifyField(const std::string& name, FieldType type, void* newValue);
	};
}