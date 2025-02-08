#pragma once

#include "../DLLDefs.h"

namespace glib
{
	/*
	* An interface class to make communication for GLFW between dlls possible.
	*/
	class GLFWInterface
	{
	public:
		GLIB_API static void WindowHint(int hint, int value);
		GLIB_API static void* GetProcAddress(const char* procname);
		GLIB_API static void MakeContextCurrent(void* handle);
	};
}