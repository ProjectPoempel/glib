#pragma once

#include "../DLLDefs.h"
#include "../math/Vec2.h"

namespace glib
{

	enum class GLIB_API FramebufferType
	{
		DEFAULT,
		THREE_D // currently not supported
	};

	class Framebuffer
	{
	private:
		void* m_BackendData;
	public:
		GLIB_API Framebuffer(void* backendData);
		GLIB_API ~Framebuffer();

		GLIB_API void Bind();
		GLIB_API void Unbind();
		GLIB_API void PrepareDraw();

		GLIB_API void* GetBackendData();
	};
}