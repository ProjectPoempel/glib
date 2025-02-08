#include "glib/window/Window.h"
#include "glib/Instance.h"
#include "glib/graphics/camera/Camera.h"
#include "glib/glibError.h"
#include "glib/graphics/Shader.h"

#include <vector>
#include <GLFW/glfw3.h>
#ifdef GLIB_WIN
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>
#include <sstream>
#include <fstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <map>
#ifdef GLIB_WIN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

extern int __GLIB_ERROR_CODE;
extern void glib_print_error();

static void _glfw_key_func(GLFWwindow* window, int key, int scancode, int action, int mods);
static void _glfw_framebuffer_resize(GLFWwindow* window, int width, int height);
static void _glfw_mousebutton(GLFWwindow* window, int button, int action, int mods);
static void _glfw_mousemove(GLFWwindow* window, double xpos, double ypos);

static int mini(int x, int y)
{
	return x < y ? x : y;
}

static int maxi(int x, int y)
{
	return x > y ? x : y;
}

static GLFWmonitor* GetCurrentMonitor(GLFWwindow* window)
{
	int nmonitors, i;
	int wx, wy, ww, wh;
	int mx, my, mw, mh;
	int overlap, bestoverlap;
	GLFWmonitor* bestmonitor;
	GLFWmonitor** monitors;
	const GLFWvidmode* mode;

	bestoverlap = 0;
	bestmonitor = NULL;

	glfwGetWindowPos(window, &wx, &wy);
	glfwGetWindowSize(window, &ww, &wh);
	monitors = glfwGetMonitors(&nmonitors);

	for (i = 0; i < nmonitors; i++) {
		mode = glfwGetVideoMode(monitors[i]);
		glfwGetMonitorPos(monitors[i], &mx, &my);
		mw = mode->width;
		mh = mode->height;

		overlap =
			maxi(0, mini(wx + ww, mx + mw) - maxi(wx, mx)) *
			maxi(0, mini(wy + wh, my + mh) - maxi(wy, my));

		if (bestoverlap < overlap) {
			bestoverlap = overlap;
			bestmonitor = monitors[i];
		}
	}

	return bestmonitor;
}

static const wchar_t* __DEFAULT_CHARSET = L"\0abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!\"~#*´§$%&/()=`'*+-\\[]{}²³<>|,.-;:_ ";

namespace glib
{
	class WindowImpl
	{
	private:
		Instance* m_Instance;
		Window* m_Wnd;
		RenderPipeline* m_Pipeline;
		std::vector<Camera*> m_Cameras;
		GLFWwindow* m_Handle;
		std::vector<Shader*> m_Shaders;
		std::map<std::string, Texture*> m_Textures;
		bool m_IsOpen;
		int m_Width;
		int m_Height;
		EventManager m_EventManager;
		SoundManager m_SoundManager;
		std::map<std::string, Font*> m_Fonts;
		bool m_FirstPipeline;
		bool m_InitSuccessful = false;
		bool m_ManageAssets;
		Camera* m_StaticCamera;
		std::vector<Camera*> m_DrawCameras;
		std::map<std::string, Model*> m_Models;
	public:
		Vec2 m_ViewportPos;
		Vec2 m_ViewportSize;
		bool m_TransparentBackground = false;
	public:
		WindowImpl(Instance* instance, Window* wnd) : m_Instance(instance), m_Wnd(wnd), m_Width(0), m_Height(0), m_IsOpen(false), m_Handle(nullptr), m_Pipeline(nullptr), m_FirstPipeline(true)
		{
		}

		void Init(const std::string& title, int width, int height, const WindowInitParams& params)
		{
			m_Width = width;
			m_Height = height;

			m_ManageAssets = params.manageAssets;

			glfwDefaultWindowHints();
			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			m_Instance->GetBackend()->PreLoadWindow();

			glfwWindowHint(GLFW_SAMPLES, 4);
			glfwWindowHint(GLFW_DECORATED, !params.noDecorations);
			if (params.transparentBackground) glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
			if (!params.resizable) glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			m_Handle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
			glfwMakeContextCurrent(m_Handle);
			glfwSwapInterval(0);
			
			m_Instance->GetBackend()->LoadWindow(m_Handle);

			glfwSetKeyCallback(m_Handle, _glfw_key_func);
			glfwSetFramebufferSizeCallback(m_Handle, _glfw_framebuffer_resize);
			glfwSetMouseButtonCallback(m_Handle, _glfw_mousebutton);
			glfwSetCursorPosCallback(m_Handle, _glfw_mousemove);

			m_ViewportSize = Vec2(width, height);
			m_Instance->GetBackend()->SetViewport(GLIB_VEC2_ZERO, m_ViewportSize);

			glfwSetWindowUserPointer(m_Handle, this);

			const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowPos(m_Handle, vidMode->width / 2 - width / 2, vidMode->height / 2 - height / 2);

			m_Pipeline = m_Instance->CreateDefaultPipeline(m_Wnd);
			m_Cameras.push_back(new Camera(GetInitialSize()));

			m_StaticCamera = new Camera(GetInitialSize());

			m_DrawCameras.push_back(GetDefaultCamera());
			m_DrawCameras.push_back(m_StaticCamera);

			m_SoundManager.ChangeOutputDevice("");
			if (Error::GetLastErrorCode() != GLIB_NO_ERROR)
			{
				glfwDestroyWindow(m_Handle);
				glib_print_error();
				std::cout << "glib Error: Default sound device does not support OpenAL Extension \"EFX\"" << std::endl;
				return;
			}

			m_IsOpen = true;

			m_InitSuccessful = true;
			Draw();
		}

		~WindowImpl()
		{
			if (!m_InitSuccessful) return;
			for (Camera* cam : m_Cameras)
			{
				delete cam;
			}
			ClearCaches(true);
			
			delete m_Pipeline;

			glfwDestroyWindow(m_Handle);
		}

		const Vec2 GetSize() const
		{
			return Vec2();
		}

		void SetVisible(bool visible)
		{
			if (visible)
			{
				glfwShowWindow(m_Handle);
			}
			else
			{
				glfwHideWindow(m_Handle);
			}
		}

		RenderPipeline* GetRenderPipeline()
		{
			return m_Pipeline;
		}

		void Draw() const
		{
			glfwMakeContextCurrent(m_Handle);

			if (m_TransparentBackground)
			{
				m_Instance->GetBackend()->ClearFramebuffer({ 0.0f, 0.0f, 0.0f, 0.0f });
			}
			else
			{
				m_Instance->GetBackend()->ClearFramebuffer({ 0.0f, 0.0f, 0.0f, 1.0f });
			}

			m_Pipeline->Flush(m_DrawCameras);

			glfwSwapBuffers(m_Handle);
		}

		bool IsOpen() const
		{
			return m_IsOpen;
		}
		
		void Update(float delta)
		{
			glfwMakeContextCurrent(m_Handle);
			for (Camera* camera : m_DrawCameras)
			{
				camera->Update(delta);
			}
			m_Pipeline->Update(delta);
			m_SoundManager.Update(delta);
		}

		void UpdateEvents(float delta)
		{
			glfwPollEvents();
			if (glfwWindowShouldClose(m_Handle))
			{
				m_IsOpen = false;
			}
		}

		Shader* LoadShader(const std::string& vertexCode, const std::string& fragmentCode)
		{
			Shader* shd = m_Instance->GetBackend()->CreateShader(vertexCode, fragmentCode);
			if (Error::GetLastErrorCode() != GLIB_NO_ERROR && !shd)
			{
				return nullptr;
			}

			if (m_ManageAssets)
			{
				m_Shaders.push_back(shd);
			}

			return shd;
		}

		Camera* GetDefaultCamera()
		{
			if (m_Cameras.empty()) return nullptr;
			return m_Cameras[0];
		}

		const Vec2 GetInitialSize() const
		{
			return Vec2(m_Width, m_Height);
		}

		Texture* LoadTexture(const std::string& path, bool pixelart)
		{
			if (m_Textures.count(path) > 0)
			{
				return m_Textures.at(path);
			}

			stbi_set_flip_vertically_on_load(false);

			int width, height, numChannels;

			stbi_uc* data = stbi_load(path.c_str(), &width, &height, &numChannels, 4);
			if (stbi_failure_reason())
			{
				Error::Throw(GLIB_FAIL_FILE_READ, "Cannot load texture file: " + path + " (" + stbi_failure_reason() + ")");
				return nullptr;
			}

			Texture* tex = LoadTextureFromRawData({ ImageChannels::RGBA, width, height, data}, pixelart);

			if (m_ManageAssets)
			{
				m_Textures.insert({ path, tex });
			}

			stbi_image_free(data);
			return tex;
		}

		ImageData LoadTextureRaw(const std::string& path)
		{
			stbi_set_flip_vertically_on_load(false);

			int width, height, numChannels;

			stbi_uc* data = stbi_load(path.c_str(), &width, &height, &numChannels, 4);
			if (stbi_failure_reason())
			{
				std::cout << stbi_failure_reason() << " (" << path << ")" << std::endl;
				return {};
			}

			return { GetImageChannelsFromNum(numChannels), width, height, data};
		}

		EventManager& GetEventManager()
		{
			return m_EventManager;
		}

		SoundManager& GetSoundManager()
		{
			return m_SoundManager;
		}

		void AddCamera(Camera* camera)
		{
			m_Cameras.push_back(camera);
			m_DrawCameras.clear();
			for (Camera* cam : m_Cameras) m_DrawCameras.push_back(cam);
			m_DrawCameras.push_back(m_StaticCamera);
		}

		void RemoveCamera(Camera* camera)
		{
			m_Cameras.erase(std::find(m_Cameras.begin(), m_Cameras.end(), camera));
			m_DrawCameras.clear();
			for (Camera* cam : m_Cameras) m_DrawCameras.push_back(cam);
			m_DrawCameras.push_back(m_StaticCamera);
		}

		const Vec2& GetViewportPos()
		{
			return m_ViewportPos;
		}

		const Vec2& GetViewportSize()
		{
			return m_ViewportSize;
		}

		Texture* LoadTextureFromRawData(ImageData data, bool pixelart)
		{
			ImageData d = data;
			d.pixelart = pixelart;
			return m_Instance->GetBackend()->CreateTexture(d);
		}

		void SetToCurrentContext()
		{
			glfwMakeContextCurrent(m_Handle);
		}

		void SetNotToCurrentContext()
		{
			glfwMakeContextCurrent(NULL);
		}

		void HideCursor()
		{
			glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		void ShowCursor()
		{
			glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		Font* LoadFont(const std::string& path, wchar_t* charset, size_t charsetLen, int size, bool pixelart)
		{
			if (m_Fonts.contains(path) && m_Fonts.at(path)->GetSize() == size)
			{
				return m_Fonts.at(path);
			}

			glfwMakeContextCurrent(m_Handle);
			
			Font* fnt = new Font(path, charset, charsetLen, size, pixelart);

			m_Fonts.insert({ path , fnt });
			return fnt;
		}

		void ClearCaches(bool del)
		{
			if (!m_ManageAssets) return;

			if (del)
			{
				for (Shader* shd : m_Shaders)
				{
					delete shd;
				}
				for (const auto& v : m_Textures)
				{
					delete v.second;
				}
				for (const auto& v : m_Fonts)
				{
					delete v.second;
				}
				for (const auto& v : m_Models)
				{
					delete v.second;
				}
			}

			m_Shaders.clear();
			m_Textures.clear();
			m_Fonts.clear();
			m_Models.clear();
		}

		void SetRenderPipeline(RenderPipeline* pipeline)
		{
			if (m_Pipeline != nullptr)
			{
				delete m_Pipeline;
			}
			m_Pipeline = pipeline;

			WindowResizeEvent event{};
			event.id = GLIB_EVENT_WINDOW_RESIZE;
			event.x = m_ViewportPos.x;
			event.y = m_ViewportPos.y;
			event.width = m_ViewportSize.x;
			event.height = m_ViewportSize.y;

			m_EventManager.CallEvent(event);
		}

		void SetIcon(const std::string& path)
		{
			GLFWimage icons[1]{};
			icons[0].pixels = stbi_load(path.c_str(), &icons[0].width, &icons[0].height, nullptr, 4);
			glfwSetWindowIcon(m_Handle, 1, icons);
			stbi_image_free(icons[0].pixels);
		}

		Camera* GetStaticCamera()
		{
			return m_StaticCamera;
		}

		void SetFullscreen(bool enabled)
		{
			GLFWmonitor* monitor = GetCurrentMonitor(m_Handle);
			if (enabled)
			{
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);
				glfwSetWindowMonitor(m_Handle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			}
			else
			{
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);
				glfwSetWindowMonitor(m_Handle, NULL, (mode->width - m_Width) / 2, (mode->height - m_Height) / 2, m_Width, m_Height, 0);
			}
		}

		Texture* LoadTextureFromPackage(const std::string& packagePath, const std::string& path, bool pixelart)
		{
			if (m_Textures.count(path) > 0)
			{
				return m_Textures.at(path);
			}

			stbi_set_flip_vertically_on_load(false);

			int width, height, numChannels;

			apkg::FileData fd = apkg::UnpackOnce(packagePath, path);
			stbi_uc* data = stbi_load_from_memory((stbi_uc*)fd.buf, fd.bufLen, &width, &height, &numChannels, 4);
			if (stbi_failure_reason())
			{
				std::cout << stbi_failure_reason() << " (" << path << ")" << std::endl;
				return nullptr;
			}

			Texture* tex = LoadTextureFromRawData({ ImageChannels::RGBA, width, height, data}, pixelart);

			if (m_ManageAssets)
			{
				m_Textures.insert({ path, tex });
			}

			stbi_image_free(data);
			delete[] fd.buf;
			return tex;
		}

		void SetIconFromPackage(const std::string& packagePath, const std::string& path)
		{
			GLFWimage icons[1]{};
			apkg::FileData fd = apkg::UnpackOnce(packagePath, path);
			icons[0].pixels = stbi_load_from_memory((stbi_uc*)fd.buf, fd.bufLen, &icons[0].width, &icons[0].height, nullptr, 4);
			glfwSetWindowIcon(m_Handle, 1, icons);
			stbi_image_free(icons[0].pixels);
			delete[] fd.buf;
		}

		Font* LoadFontFromPackage(const std::string& packagePath, const std::string& path, wchar_t* charset, size_t charsetLen, int size, bool pixelart)
		{
			if (m_Fonts.contains(path) && m_Fonts.at(path)->GetSize() == size)
			{
				return m_Fonts.at(path);
			}

			glfwMakeContextCurrent(m_Handle);

			Font* fnt = new Font(packagePath, path, charset, charsetLen, size, pixelart);

			m_Fonts.insert({ path , fnt });
			return fnt;
		}

		void* GetHandle()
		{
			return m_Handle;
		}

		void LockCursor()
		{
			glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		void UnlockCursor()
		{
			glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		Model* LoadModel(const std::string& path, bool pixelart)
		{
			if (m_Models.count(path) > 0) return m_Models.at(path);
			Model* model = Model::LoadModel(path, pixelart);
			m_Models.insert({ path, model });
			return model;
		}

		Instance* GetInstance()
		{
			return m_Instance;
		}

		Shader* LoadSpriteShader(const std::string& fragmentCode)
		{
			Shader* shd = m_Instance->GetBackend()->CreateSpriteShader(fragmentCode);
			if (Error::GetLastErrorCode() != GLIB_NO_ERROR && !shd)
			{
				return nullptr;
			}

			return shd;
		}
	};
}

using namespace glib;

static void _glfw_key_func(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	WindowImpl* wnd = (WindowImpl*) glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS)
	{
		KeyEvent e{};
		e.id = GLIB_EVENT_KEY_PRESS;
		e.key = (KeyCode)key;
		wnd->GetEventManager().CallEvent(e);
	}
	else if (action == GLFW_RELEASE)
	{
		KeyEvent e{};
		e.id = GLIB_EVENT_KEY_RELEASE;
		e.key = (KeyCode)key;
		wnd->GetEventManager().CallEvent(e);
	}
}

static void _glfw_framebuffer_resize(GLFWwindow* window, int width, int height)
{
	WindowImpl* wnd = (WindowImpl*)glfwGetWindowUserPointer(window);
	Vec2 size = wnd->GetInitialSize();

	if (width < 1 || height < 1) return;

	float ratioX = width / (float)size.x;
	float ratioY = height / (float)size.y;
	float ratio = fminf(ratioX, ratioY);

	int viewWidth = (int)(size.x * ratio);
	int viewHeight = (int)(size.y * ratio);

	int viewX = (int)(width - size.x * ratio) / 2;
	int viewY = (int)(height - size.y * ratio) / 2;
	
	wnd->m_ViewportPos = Vec2(viewX, viewY);
	wnd->m_ViewportSize = Vec2(viewWidth, viewHeight);

	WindowResizeEvent event{};
	event.id = GLIB_EVENT_WINDOW_RESIZE;
	event.x = viewX;
	event.y = viewY;
	event.width = viewWidth;
	event.height = viewHeight;

	wnd->GetEventManager().CallEvent(event);
}

void _glfw_mousebutton(GLFWwindow* window, int button, int action, int mods)
{
	WindowImpl* wnd = (WindowImpl*)glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS)
	{
		MouseEvent e{};
		e.id = GLIB_EVENT_MOUSE_PRESS;
		e.btn = (MouseButton)button;
		wnd->GetEventManager().CallEvent(e);
	}
	else if (action == GLFW_RELEASE)
	{
		MouseEvent e{};
		e.id = GLIB_EVENT_MOUSE_RELEASE;
		e.btn = (MouseButton)button;
		wnd->GetEventManager().CallEvent(e);
	}
}

void _glfw_mousemove(GLFWwindow* window, double xpos, double ypos)
{
	WindowImpl* wnd = (WindowImpl*)glfwGetWindowUserPointer(window);
	MouseMoveEvent e{};
	e.id = GLIB_EVENT_MOUSE_MOVE;
	e.x = xpos;
	e.y = ypos;
	wnd->GetEventManager().CallEvent(e);
}

glib::Window::Window(Instance* instance, const std::string& title, int width, int height, const WindowInitParams& params)
{
	impl = new WindowImpl(instance, this);
	impl->Init(title, width, height, params);
}

glib::Window::~Window()
{
	delete impl;
}

void glib::Window::SetVisible(bool visible)
{
	impl->SetVisible(visible);
}

const Vec2 glib::Window::GetSize() const
{
	return impl->GetSize();
}

const Vec2 glib::Window::GetInitialSize() const
{
	return impl->GetInitialSize();
}

void glib::Window::SetIconFromPackage(const std::string& packagePath, const std::string& path)
{
	impl->SetIconFromPackage(packagePath, path);
}

void glib::Window::SetRenderPipeline(RenderPipeline* pipeline)
{
	impl->SetRenderPipeline(pipeline);
}

RenderPipeline* glib::Window::GetRenderPipeline()
{
	return impl->GetRenderPipeline();
}

void glib::Window::Draw() const
{
	impl->Draw();
}

bool glib::Window::IsOpen() const
{
	return impl->IsOpen();
}

void glib::Window::Update(float delta)
{
	impl->Update(delta);
}

void glib::Window::UpdateEvents(float delta)
{
	impl->UpdateEvents(delta);
}

Camera* glib::Window::GetDefaultCamera()
{
	return impl->GetDefaultCamera();
}

EventManager& glib::Window::GetEventManager()
{
	return impl->GetEventManager();
}

SoundManager& glib::Window::GetSoundManager()
{
	return impl->GetSoundManager();
}

const Vec2& glib::Window::GetViewportPos()
{
	return impl->GetViewportPos();
}

const Vec2& glib::Window::GetViewportSize()
{
	return impl->GetViewportSize();
}

Shader* glib::Window::LoadShader(const std::string& vertexCode, const std::string& fragmentCode)
{
	return impl->LoadShader(vertexCode, fragmentCode);
}

Shader* glib::Window::LoadShaderFromFiles(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		return LoadShader(vertexCode, fragmentCode);
	}
	catch (std::ifstream::failure e)
	{
		__GLIB_ERROR_CODE = GLIB_FAIL_FILE_READ;
		glib_print_error();
		return nullptr;
	}
}

Shader* glib::Window::LoadSpriteShader(const std::string& fragmentCode)
{
	return impl->LoadSpriteShader(fragmentCode);
}

Shader* glib::Window::LoadSpriteShaderFromFile(const std::string& fragmentPath)
{
	std::string fragmentCode;
	std::ifstream fShaderFile;

	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		fShaderStream << fShaderFile.rdbuf();

		fShaderFile.close();

		fragmentCode = fShaderStream.str();

		return LoadSpriteShader(fragmentCode);
	}
	catch (std::ifstream::failure e)
	{
		__GLIB_ERROR_CODE = GLIB_FAIL_FILE_READ;
		glib_print_error();
		return nullptr;
	}
}

Texture* glib::Window::LoadTexture(const std::string& path, bool pixelart)
{
	return impl->LoadTexture(path, pixelart);
}

Texture* glib::Window::LoadTextureFromPackage(const std::string& packagePath, const std::string& path, bool pixelart)
{
	return impl->LoadTextureFromPackage(packagePath, path, pixelart);
}

ImageData glib::Window::LoadTextureRaw(const std::string& path)
{
	return impl->LoadTextureRaw(path);
}

Texture* glib::Window::LoadTextureFromRawData(ImageData data, bool pixelart)
{
	return impl->LoadTextureFromRawData(data, pixelart);
}

Font* glib::Window::LoadFont(const std::string& path, int size, bool pixelart)
{
	return LoadFont(path, (wchar_t*)__DEFAULT_CHARSET, 101, size, pixelart);
}

Font* glib::Window::LoadFont(const std::string& path, wchar_t* charset, size_t charsetLen, int size, bool pixelart)
{
	return impl->LoadFont(path, charset, charsetLen, size, pixelart);
}

Font* glib::Window::LoadFontFromPackage(const std::string& packagePath, const std::string& path, int size, bool pixelart)
{
	return LoadFontFromPackage(packagePath, path, (wchar_t*)__DEFAULT_CHARSET, 101, size, pixelart);
}

Font* glib::Window::LoadFontFromPackage(const std::string& packagePath, const std::string& path, wchar_t* charset, size_t charsetLen, int size, bool pixelart)
{
	return impl->LoadFontFromPackage(packagePath, path, charset, charsetLen, size, pixelart);
}

Model* glib::Window::LoadModel(const std::string& path, bool pixelart)
{
	return impl->LoadModel(path, pixelart);
}

void glib::Window::SetToCurrentContext()
{
	impl->SetToCurrentContext();
}

void glib::Window::SetNotToCurrentContext()
{
	impl->SetNotToCurrentContext();
}

void glib::Window::HideCursor()
{
	impl->HideCursor();
}

void glib::Window::ShowCursor()
{
	impl->ShowCursor();
}

void glib::Window::LockCursor()
{
	impl->LockCursor();
}

void glib::Window::UnlockCursor()
{
	impl->UnlockCursor();
}

void glib::Window::SetIcon(const std::string& path)
{
	impl->SetIcon(path);
}

void glib::Window::AddCamera(Camera* camera)
{
	impl->AddCamera(camera);
}

void glib::Window::RemoveCamera(Camera* camera)
{
	impl->RemoveCamera(camera);
}

Camera* glib::Window::GetStaticCamera()
{
	return impl->GetStaticCamera();
}

void glib::Window::ClearCaches()
{
	impl->ClearCaches(false);
}

void glib::Window::ClearCachesDelete()
{
	impl->ClearCaches(true);
}

void glib::Window::SetFullscreen(bool enabled)
{
	impl->SetFullscreen(enabled);
}

void glib::Window::SetPosition(const Vec2& position)
{
	glfwSetWindowPos((GLFWwindow*)impl->GetHandle(), position.x, position.y);
}

void glib::Window::SetSize(const Vec2& size)
{
	glfwSetWindowSize((GLFWwindow*)impl->GetHandle(), size.x, size.y);
}

void glib::Window::Center(glib::Axis axis)
{
	int width, height;
	glfwGetWindowSize((GLFWwindow*)impl->GetHandle(), &width, &height);
	int x, y;
	glfwGetWindowPos((GLFWwindow*)impl->GetHandle(), &x, &y);

	const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	int _x = 0, _y = 0;
	if (axis == X || axis == XY)
	{
		_x = vidMode->width / 2 - width / 2;
	}
	if (axis == Y || axis == XY)
	{
		_y = vidMode->height / 2 - height / 2;
	}

	glfwSetWindowPos((GLFWwindow*)impl->GetHandle(), _x, _y);
}

void glib::Window::AddFlags(uint64_t flags)
{
#ifdef GLIB_WIN
	HWND hwnd = glfwGetWin32Window((GLFWwindow*)impl->GetHandle());
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
#endif

	if (flags & GLIB_WND_FLAG_RESIZE_BOX)
	{
#ifdef GLIB_WIN
		style = style | ~WS_SIZEBOX;
#endif
	}
	if (flags & GLIB_WND_FLAG_MINIMIZE_BUTTON)
	{
#ifdef GLIB_WIN
		style = style | ~WS_MINIMIZE;
#endif
	}
	if (flags & GLIB_WND_FLAG_MAXIMIZE_BUTTON)
	{
#ifdef GLIB_WIN
		style = style | ~WS_MAXIMIZEBOX;
#endif
	}
	if (flags & GLIB_WND_FLAG_TRANSPARENT)
	{
		impl->m_TransparentBackground = true;
	}

#ifdef GLIB_WIN
	SetWindowLong(hwnd, GWL_STYLE, style);
#endif
}

void glib::Window::RemoveFlags(uint64_t flags)
{
#ifdef GLIB_WIN
	HWND hwnd = glfwGetWin32Window((GLFWwindow*)impl->GetHandle());
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
#endif

	if (flags & GLIB_WND_FLAG_RESIZE_BOX)
	{
#ifdef GLIB_WIN
		style = style & ~WS_SIZEBOX;
#endif
	}
	if (flags & GLIB_WND_FLAG_MINIMIZE_BUTTON)
	{
#ifdef GLIB_WIN
		style = style & ~WS_MINIMIZE;
#endif
	}
	if (flags & GLIB_WND_FLAG_MAXIMIZE_BUTTON)
	{
#ifdef GLIB_WIN
		style = style & ~WS_MAXIMIZEBOX;
#endif
	}
	if (flags & GLIB_WND_FLAG_TRANSPARENT)
	{
		impl->m_TransparentBackground = false;
	}

#ifdef GLIB_WIN
	SetWindowLong(hwnd, GWL_STYLE, style);
#endif

}

Vec2 glib::Window::GetPosition()
{
	int x, y;
	glfwGetWindowPos((GLFWwindow*)impl->GetHandle(), &x, &y);
	return Vec2(x, y);
}

Vec2 glib::Window::GetSize()
{
	int width, height;
	glfwGetWindowSize((GLFWwindow*)impl->GetHandle(), &width, &height);
	return Vec2(width, height);
}

Vec2 glib::Window::GetCurrentScreenResolution()
{
	const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	return Vec2(vidMode->width, vidMode->height);
}

float glib::Window::GetTime()
{
	return glfwGetTime();
}

Instance* glib::Window::GetInstance()
{
	return impl->GetInstance();
}

void* glib::Window::GetHandle()
{
	return impl->GetHandle();
}