#include "glib/Instance.h"
#include "glib/graphics/pipeline/CameraRenderer.h"
#include "glib/graphics/pipeline/WindowRenderer.h"
#include "glib/utils/glibGlobals.h"

#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace glib;

extern bool __GLIB_INIT_WITH_EXISTING;

namespace glib
{
	class InstanceImpl
	{
	private:
		Instance* m_Instance;
		std::vector<Window*> m_Windows;
		bool m_InitFailed = false;
		Backend* m_Backend;
	public:
		InstanceImpl(Instance* instance, Backend* backend) : m_Instance(instance), m_Backend(backend)
		{
			if (!__GLIB_INIT_WITH_EXISTING) {
				if (glfwInit() == GLFW_FALSE)
				{
					m_InitFailed = true;
					Error::Throw(GLIB_FAIL_INIT_GLFW, "Failed to initialize GLFW");
					return;
				}
			}
		}

		~InstanceImpl()
		{
			if (m_InitFailed) return;


			for (Window* wnd : m_Windows)
			{
				delete wnd;
			}
			
			Backend::UnloadBackend(m_Backend);
			delete m_Backend;
			
			if (!__GLIB_INIT_WITH_EXISTING) glfwTerminate();
		}

		Window* CreateWindow(const std::string& title, int width, int height, const WindowInitParams& params)
		{
			Window* wnd = new Window(m_Instance, title, width, height, params);
			if (Error::GetLastErrorCode() != GLIB_NO_ERROR)
			{
				delete wnd;
				return nullptr;
			}
			if (m_Windows.size() == 0)
			{
				gWindow = wnd;
			}
			m_Windows.push_back(wnd);
			return wnd;
		}

		double GetTime() const
		{
			return glfwGetTime();
		}

		Backend* GetBackend()
		{
			return m_Backend;
		}
	};
}

glib::Instance::Instance(Backend* backend)
{
	impl = new InstanceImpl(this, backend);
}

glib::Instance::~Instance()
{
	delete impl;
}

Window* glib::Instance::CreateWindow(const std::string& title, int width, int height, const WindowInitParams& params)
{
	return impl->CreateWindow(title, width, height, params);
}

RenderPipeline* glib::Instance::CreateDefaultPipeline(Window* wnd) const
{
	RenderPipeline* pipeline = new RenderPipeline(wnd);
	pipeline->AddElement("cam", new CameraRenderer);
	pipeline->AddElement("wnd", new WindowRenderer);
	return pipeline;
}

double glib::Instance::GetTime() const
{
	return impl->GetTime();
}

Backend* glib::Instance::GetBackend()
{
	return impl->GetBackend();
}
