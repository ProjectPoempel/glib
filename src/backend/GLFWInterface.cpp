#include "glib/backend/GLFWInterface.h"

#include <GLFW/glfw3.h>

void glib::GLFWInterface::WindowHint(int hint, int value)
{
	glfwWindowHint(hint, value);
}

void* glib::GLFWInterface::GetProcAddress(const char* procname)
{
	return (void*)glfwGetProcAddress(procname);
}

void glib::GLFWInterface::MakeContextCurrent(void* handle)
{
	glfwMakeContextCurrent((GLFWwindow*)handle);
}
