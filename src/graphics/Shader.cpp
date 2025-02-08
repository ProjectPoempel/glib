#include "glib/graphics/Shader.h"
#include "glib/utils/glibGlobals.h"

namespace glib
{
	class ShaderImpl
	{
	private:
		Shader* m_Parent;
		void* m_BackendData;
	public:
		ShaderImpl(Shader* parent, void* backendData) : m_Parent(parent), m_BackendData(backendData)
		{
		}

		void Use() const
		{
			gInstance->GetBackend()->UseShader(m_Parent);
		}

		void SetMat4(const std::string& name, Mat4& mat)
		{
			gInstance->GetBackend()->SetShader(m_Parent, name, mat);
		}

		void SetInt(const std::string& name, int i)
		{
			gInstance->GetBackend()->SetShader(m_Parent, name, i);
		}

		void SetColor(const std::string& name, const Color& color)
		{
			gInstance->GetBackend()->SetShader(m_Parent, name, color);
		}

		void SetFloat(const std::string& name, float f)
		{
			gInstance->GetBackend()->SetShader(m_Parent, name, f);
		}

		void SetVec2(const std::string& name, const Vec2& vec)
		{
			gInstance->GetBackend()->SetShader(m_Parent, name, vec);
		}

		void* GetBackendData()
		{
			return m_BackendData;
		}
	};
}

using namespace glib;

glib::Shader::Shader(void* backendData)
{
	impl = new ShaderImpl(this, backendData);
}

glib::Shader::~Shader()
{
	delete impl;
	gInstance->GetBackend()->DeleteShader(this);
}

void glib::Shader::Use() const
{
	impl->Use();
}

void glib::Shader::SetMat4(const std::string& name, Mat4& mat)
{
	impl->SetMat4(name, mat);
}

void glib::Shader::SetInt(const std::string& name, int i)
{
	impl->SetInt(name, i);
}

void glib::Shader::SetColor(const std::string& name, const Color& color)
{
	impl->SetColor(name, color);
}

void glib::Shader::SetFloat(const std::string& name, float f)
{
	impl->SetFloat(name, f);
}

void glib::Shader::SetVec2(const std::string& name, const Vec2& vec)
{
	impl->SetVec2(name, vec);
}

void* glib::Shader::GetBackendData()
{
	return impl->GetBackendData();
}
