#include "glib/framebuffer/Framebuffer.h"
#include "glib/glibError.h"
#include "glib/utils/glibGlobals.h"

glib::Framebuffer::Framebuffer(void* backendData) : m_BackendData(backendData)
{
}

glib::Framebuffer::~Framebuffer()
{
	gInstance->GetBackend()->DeleteFramebuffer(this);
}

void glib::Framebuffer::Bind()
{
	gInstance->GetBackend()->BindFramebuffer(this);
}

void glib::Framebuffer::Unbind()
{
	gInstance->GetBackend()->UnbindFramebuffer(this);
}

void glib::Framebuffer::PrepareDraw()
{
	gInstance->GetBackend()->DrawFramebuffer(this);
}

void* glib::Framebuffer::GetBackendData()
{
	return m_BackendData;
}

