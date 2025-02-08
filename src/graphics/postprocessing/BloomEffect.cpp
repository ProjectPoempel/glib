#include "glib/graphics/postprocessing/BloomEffect.h"
#include "glib/window/Window.h"
#include "glib/Instance.h"
#include "glib/backend/PostProcessingBackend.h"

using namespace glib;

glib::BloomEffect::BloomEffect() : PostProcessingEffect("void main() {}")
{
}

glib::BloomEffect::~BloomEffect()
{
    delete m_LFB;
    delete m_MFB;
}

const PipelineData glib::BloomEffect::Downstream(const PipelineData data)
{
    if (data.fb == nullptr) return data;

    Backend* backend = m_Wnd->GetInstance()->GetBackend();
    Camera* cam = (Camera*)data.ptr;
    if (cam == nullptr)
    {
        return data;
    }

    backend->NoBlending();
    backend->ClearFramebuffer(GLIB_COLOR_BLACK);

    BloomPassData _data{};
    _data.fb1 = m_FB;
    _data.fb2 = m_LFB;
    _data.fb4 = m_MFB;
    _data.fb3 = data.fb;
    _data.threshold = threshold;
    _data.resolution = m_Size;

    // Perform passes
    for (int i = 1; i <= 4; i++)
    {
        backend->GetPostProcessingBackend()->PerformBloomPass(i, _data);
    }
    
    PipelineData newData = data;
    newData.fb = m_FB;
    return newData;
}

void glib::BloomEffect::ConstructFBO(Vec2 pos, Vec2 size)
{
    PipelineRenderer::ConstructFBO(pos, size);

    if (m_LFB == nullptr) delete m_LFB;
    m_LFB = m_Wnd->GetInstance()->GetBackend()->CreateFramebuffer(FramebufferType::DEFAULT, m_Size, true);

    if (m_MFB == nullptr) delete m_MFB;
    m_MFB = m_Wnd->GetInstance()->GetBackend()->CreateFramebuffer(FramebufferType::DEFAULT, m_Size, true);
}
