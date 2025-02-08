#include "glib/backend/Backend.h"
#include "glib/glibError.h"

#include <iostream>

#ifdef GLIB_WIN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef GLIB_LINUX
#include <dlfcn.h>
#endif

using namespace glib;

Backend* glib::Backend::GetBackend(const std::string& name)
{
    std::string path;
#ifdef GLIB_WIN
    path = "backend-" + name + ".dll";
#endif
#ifdef GLIB_LINUX
    path = "./libbackend-" + name + ".so";
#endif

std::cout << path << std::endl;

#ifdef GLIB_WIN
    HMODULE module = LoadLibraryA(path.c_str());
    if (!module) {
        Error::Throw(GLIB_BACKEND_NOT_FOUND, "Couldn't find dll for backend: " + name);
        return nullptr;
    }

    FARPROC addr = GetProcAddress(module, GLIB_BACKEND_INIT_NAME);
    if (!addr) {
        std::cout << GetLastError() << std::endl;
        Error::Throw(GLIB_BACKEND_INIT_ERROR, "No init function in backend: " + name);
        FreeLibrary(module);
        return nullptr;
    }
#endif
#ifdef GLIB_LINUX
    char* err = nullptr;
    
    void* handle = dlopen(path.c_str(), RTLD_NOW);
    if (!handle)
    {
        Error::Throw(GLIB_BACKEND_NOT_FOUND, "Couldn't find shared library for backend: " + name + " (" + dlerror() + ")");
        return nullptr;
    }

    dlerror();

    void* addr = dlsym(handle, GLIB_BACKEND_INIT_NAME);
    if ((err = dlerror()))
    {
        Error::Throw(GLIB_BACKEND_INIT_ERROR, "No init function in backend: " + name + " (" + err + ")");
        return nullptr;
    }
#endif

    BackendInitFunc func = (BackendInitFunc)addr;
    Backend* backend = func(GLIB_VERSION);
    if (!backend)
    {
        Error::Throw(GLIB_BACKEND_INIT_ERROR, "Failed to initialize backend: " + name);
#ifdef GLIB_WIN
        FreeLibrary(module);
#endif
#ifdef GLIB_LINUX
        dlclose(handle);
#endif
        return nullptr;
    }

    if (Error::GetLastErrorCode() != GLIB_NO_ERROR)
    {
        Error::Throw(GLIB_BACKEND_INIT_ERROR, "Error while initializing backend: " + name);
#ifdef GLIB_WIN
        FreeLibrary(module);
#endif
#ifdef GLIB_LINUX
        dlclose(handle);
#endif
        return nullptr;
    }

#ifdef GLIB_WIN
    backend->m_LibHandle = module;
#endif
#ifdef GLIB_LINUX
    backend->m_LibHandle = handle;        
#endif

    return backend;
}

void glib::Backend::UnloadBackend(Backend* backend)
{
    backend->Unload();
#ifdef GLIB_WIN
    FreeLibrary((HMODULE)backend->m_LibHandle);
#endif
#ifdef GLIB_LINUX
    dlclose(backend->m_LibHandle);
#endif
}
