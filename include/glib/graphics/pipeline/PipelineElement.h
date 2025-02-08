#pragma once

#include "../../math/Vec2.h"
#include "../camera/Camera.h"
#include "../../framebuffer/Framebuffer.h"
#include <cstdint>
#include <vector>

namespace glib
{
	class Window;

	struct PipelineData
	{
		const void* ptr;
		Framebuffer* fb;
		int i;
		float f;
		Window* wnd;
		void* ptr2;
		void* ptr3;
		void* ptr4;
		bool isFinal;
		bool camPositioned;
	};

	class PipelineElement
	{
	public:
		Vec2 viewportPos;
		Vec2 viewportSize;
		std::vector<Camera*> m_ExcludedCameras;
	public:
		virtual ~PipelineElement();

		GLIB_API void AddExcludedCamera(Camera* camera);
		GLIB_API void RemoveExcludedCamera(Camera* camera);
		GLIB_API bool IsExcludedFromCamera(Camera* camera);

		virtual void Construct(Window* wnd) = 0;
		virtual const PipelineData Downstream(const PipelineData data) = 0;
	};
}