#pragma once

#include "../../DLLDefs.h"
#include "../camera/Camera.h"
#include "PipelineElement.h"

#include <vector>

namespace glib
{
	class Window;

	class RenderPipeline
	{
	protected:
		std::vector<std::pair<std::string, PipelineElement*>> m_Elements;
		Window* m_Wnd;
	public:
		GLIB_API RenderPipeline(Window* wnd);
		GLIB_API virtual ~RenderPipeline();

		GLIB_API virtual void Update(float delta);

		GLIB_API virtual void Flush(const std::vector<Camera*>& cameras) const;
		GLIB_API virtual void AddElement(const std::string& name, PipelineElement* element);
		GLIB_API virtual void AddElementBefore(const std::string& other, const std::string& name, PipelineElement* element);
		GLIB_API virtual void AddElementAfter(const std::string& other, const std::string& name, PipelineElement* element);
		GLIB_API virtual void RemoveElement(const std::string& name);
		GLIB_API virtual PipelineElement* GetElement(const std::string& name) const;
	};
}