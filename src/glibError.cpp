#include "glib/glibError.h"

#include <iostream>

using namespace glib;

static ErrorCode __last_code = GLIB_NO_ERROR;
static std::string __last_message;
static ErrorCallback __error_callback;

static void Reset()
{
	__last_code = GLIB_NO_ERROR;
	__last_message.clear();
}

void glib::Error::Throw(ErrorCode code, const std::string& message)
{
	Throw(code, message, "", "");
}

void glib::Error::Throw(ErrorCode code, const std::string& message, const std::string& clazz, const std::string& func)
{
	__last_code = code;
	__last_message = message;

	if (__error_callback)
	{
		__error_callback(code, message, clazz, func);
	}
	else
	{
		if (clazz.empty() || func.empty())
		{
			std::cout << "[glib::Error]: 0x" << std::hex << code << ": " << message << std::endl;
		}
		else
		{
			std::cout << "[glib::Error]: (" << clazz << "::" << func << "()) 0x" << std::hex << code << ": " << message << std::endl;
		}
	}
}

ErrorCode glib::Error::GetLastErrorCode(bool reset)
{
	ErrorCode code = __last_code;
	if (reset) Reset();
	return code;
}

const std::string& glib::Error::GetLastErrorMessage(bool reset)
{
	std::string message = __last_message;
	if (reset) Reset();
	return message;
}

void glib::Error::SetCallback(ErrorCallback callback)
{
	__error_callback = callback;
}
