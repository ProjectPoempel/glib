#pragma once

// DLL Defines

constexpr auto GLIB_VERSION = "0.0.1";

#ifdef _WIN32
#define GLIB_WIN
#endif

#ifdef linux
#define GLIB_LINUX
#endif

#ifdef GLIB_WIN
#ifdef GLIBDLL_EXPORTS
#define GLIB_API __declspec(dllexport)
#else
#define GLIB_API __declspec(dllimport)
#endif
#else
#define GLIB_API 
#endif