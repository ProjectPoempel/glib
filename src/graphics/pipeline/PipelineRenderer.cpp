#include "glib/graphics/pipeline/PipelineRenderer.h"
#include "glib/graphics/camera/Camera.h"
#include "glib/window/Window.h"
#include "glib/Instance.h"

namespace glib
{
	class RendererListener : public EventSubscriber
	{
	private:
		PipelineRenderer* m_R;
	public:
		RendererListener(PipelineRenderer* r) : m_R(r)
		{
		}

		void OnWindowResize(const WindowResizeEvent& event) override
		{
			m_R->ConstructFBO(Vec2(event.x, event.y), Vec2(event.width, event.height));
		}
	};
}

glib::PipelineRenderer::PipelineRenderer() : m_FB(nullptr), m_L(nullptr), m_Wnd(nullptr)
{
}

glib::PipelineRenderer::~PipelineRenderer()
{
	m_Wnd->GetEventManager().Unsubscribe(m_L);
	delete m_L;
	delete m_FB;
}

void glib::PipelineRenderer::Construct(Window* wnd)
{
	wnd->SetToCurrentContext();
	m_L = new RendererListener(this);
	m_Wnd = wnd;
	wnd->GetEventManager().Subscribe(GLIB_EVENT_WINDOW_RESIZE, m_L);

	Vec2 size = wnd->GetInitialSize();
	m_Size = size;
	
	ConstructFBO(Vec2(0.0f, 0.0f), size);
}

void glib::PipelineRenderer::ConstructFBO(Vec2 pos, Vec2 size)
{
	m_Wnd->SetToCurrentContext();
	if (m_FB != nullptr)
	{
		delete m_FB;
	}

	m_Size = size;
	m_Pos = pos;

	Mat4 m = Mat4::Ortho(0.0f, m_Wnd->GetInitialSize().x, m_Wnd->GetInitialSize().y, 0.0f);

	m_Wnd->GetInstance()->GetBackend()->GetSpriteShader()->Use();
	m_Wnd->GetInstance()->GetBackend()->GetSpriteShader()->SetMat4("glib_projection_matrix", m);

	m_Wnd->GetInstance()->GetBackend()->GetTextShader()->Use();
	m_Wnd->GetInstance()->GetBackend()->GetTextShader()->SetMat4("glib_projection_matrix", m);

	m_FB = m_Wnd->GetInstance()->GetBackend()->CreateFramebuffer(FramebufferType::DEFAULT, m_Size, true);
}
