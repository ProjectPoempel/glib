#include "glib/glib.h"
#include "glib/utils/glibGlobals.h"

using namespace glib;

Instance* __glib_instance = nullptr;

GINSTANCE gInstance = nullptr;
GWINDOW gWindow = nullptr;
GWINDOW gWindow1 = nullptr;
GWINDOW gWindow2 = nullptr;
GWINDOW gWindow3 = nullptr;
bool __GLIB_INIT_WITH_EXISTING = false;

int __GLIB_ERROR_CODE;
void glib_print_error()
{

}

Instance* glib::Init(Backend* backend)
{
	if (__glib_instance != nullptr) return __glib_instance;
	__glib_instance = new Instance(backend);

	if (Error::GetLastErrorCode() != GLIB_NO_ERROR)
	{
		delete __glib_instance;
		return nullptr;
	}

	gInstance = __glib_instance;

	return __glib_instance;
}

void glib::SimpleInit()
{
	Init(Backend::GetBackend("opengl"));
}

void glib::Terminate()
{
	if (__glib_instance == nullptr) return;
	delete __glib_instance;
}
