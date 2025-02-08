#pragma once

#include "../DLLDefs.h"
#include "../utils/Color.h"
#include "../graphics/Shader.h"
#include "../graphics/Texture.h"
#include "../framebuffer/Framebuffer.h"
#include "../math/Mat4.h"
#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include "GLFWInterface.h"

#include <string>

/* The function name in the backend dll/shared library to load the backend */
#define GLIB_BACKEND_INIT_NAME "glib_backend_init"
#define _GLIB_BACKEND_INIT_NAME glib_backend_init

namespace glib
{
	class Backend;
	class PostProcessingBackend;

	typedef Backend* (*BackendInitFunc)(const std::string&);

	enum class GLIB_API BlendFuncs
	{
		SRC_ONE_MINUS_SRC,
		ONE_ONE_MINUS_SRC,
		SRC_DST
	};

	enum class GLIB_API BlendMode
	{
		ADD,
		SUBTRACT,
		MULTIPLY
	};

	class Backend
	{
	private:
		void* m_LibHandle;
	public:
		GLIB_API static Backend* GetBackend(const std::string& name);
		GLIB_API static void UnloadBackend(Backend* backend);
	public: // Implementation methods
		virtual void PreLoadWindow() = 0;
		virtual void LoadWindow(void* handle) = 0;
		virtual void Unload() = 0;
		GLIB_API virtual void ClearFramebuffer(const Color& color, bool alsoDepth = false) = 0;
		GLIB_API virtual Shader* CreateShader(const std::string& vertexCode, const std::string& fragmentCode) = 0;
		GLIB_API virtual void DeleteShader(Shader* shader) = 0;
		GLIB_API virtual Texture* CreateTexture(const ImageData& data) = 0;
		GLIB_API virtual void DeleteTexture(Texture* texture) = 0;
		GLIB_API virtual Framebuffer* CreateFramebuffer(FramebufferType type, const Vec2& size, bool transparent = false) = 0;
		GLIB_API virtual void DeleteFramebuffer(Framebuffer* fb) = 0;
		GLIB_API virtual void DrawFramebuffer(Framebuffer* fb) = 0;
		GLIB_API virtual void BindFramebuffer(Framebuffer* fb) = 0;
		GLIB_API virtual void UnbindFramebuffer(Framebuffer* fb) = 0;
		GLIB_API virtual void BindTexture(Texture* texture) = 0;
		GLIB_API virtual void UnbindTexture(Texture* texture) = 0;
		GLIB_API virtual void NoBlending() = 0;
		GLIB_API virtual void SetBlending(BlendMode mode, BlendFuncs funcs) = 0;
		GLIB_API virtual void DrawQuad() = 0;
		GLIB_API virtual void DrawOutlinedQuad() = 0;
		GLIB_API virtual void SetShader(Shader* shader, const std::string& name, Mat4& mat) = 0;
		GLIB_API virtual void SetShader(Shader* shader, const std::string& name, int i) = 0;
		GLIB_API virtual void SetShader(Shader* shader, const std::string& name, const Color& color) = 0;
		GLIB_API virtual void SetShader(Shader* shader, const std::string& name, float f) = 0;
		GLIB_API virtual void SetShader(Shader* shader, const std::string& name, const Vec2& vec) = 0;
		GLIB_API virtual void SetShader(Shader* shader, const std::string& name, const Vec3& vec) = 0;
		GLIB_API virtual void UseShader(Shader* shader) = 0;
		GLIB_API virtual Shader* GetSpriteShader() = 0;
		GLIB_API virtual Shader* GetTextShader() = 0;
		GLIB_API virtual Shader* GetWindowShader() = 0;
		GLIB_API virtual void SetViewport(const Vec2& pos, const Vec2& size) = 0;
		GLIB_API virtual void DrawScreenQuad() = 0;
		GLIB_API virtual Shader* CreateSpriteShader(const std::string& fragmentCode, bool includeHeader = true) = 0;
		GLIB_API virtual Shader* CreateWindowShader(const std::string& fragmentCode) = 0;
		GLIB_API virtual std::string GetFragmentShaderHeader() = 0;
		GLIB_API virtual void SetClip(const Vec2& pos, const Vec2& size) = 0;
		GLIB_API virtual void NoClip() = 0;
		GLIB_API virtual PostProcessingBackend* GetPostProcessingBackend() = 0;
		GLIB_API virtual Shader* CreateHeadedShader(const std::string& vertexCode, const std::string& fragmentCode) = 0;
	};
}