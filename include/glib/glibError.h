#pragma once

#include "DLLDefs.h"

#include <string>
#include <cstdint>
#include <functional>

#define GLIB_SUCCESS 0

// Error codes

#define GLIB_NO_ERROR -1
#define GLIB_ERROR_NULLPTR 1
#define GLIB_ERROR_INVALID_OPERATION 2

/* Failed to initialize glfw */
#define GLIB_FAIL_INIT_GLFW 0x10

/* Failed to read a file */
#define GLIB_FAIL_FILE_READ 0x20

/* Failed to link vertex and fragment shader */
#define GLIB_FAIL_SHADER_LINK 0x30
/* Failed to compile vertex shader */
#define GLIB_FAIL_SHADER_COMPILEv 0x31
/* Failed to compile fragment shader */
#define GLIB_FAIL_SHADER_COMPILEf 0x32

/* Unimplemented Framebuffer type */
#define GLIB_FRAMEBUFFER_UNKNOWN_TYPE 0x40
/* Something went wrong while constructing OpenGL Framebuffer */
#define GLIB_FRAMEBUFFER_NOT_COMPLETE 0x41

/* Failed to load sound file */
#define GLIB_SOUND_FILE_READ_FAIL 0x50
/* Unsupported audio format (mostly occurrs when a sound file has more than 2 channels) */
#define GLIB_SOUND_UNSUPPORTED_FORMAT 0x51
/* Failed to open / access sound file */
#define GLIB_SOUND_FILE_OPEN_FAIL 0x52
/* Internal error */
#define GLIB_SOUND_INTERNAL_ERROR 0x53
/* Device doesn't support the OpenAL Effects extension */
#define GLIB_SOUND_OPENAL_EXT_EFX_NOT_SUPPORTED 0x54

#define GLIB_BACKEND_NOT_FOUND 0x60
#define GLIB_BACKEND_INIT_ERROR 0x61

#define GLIB_REFLECT_NO_SUCH_FIELD 0x70
#define GLIB_REFLECT_INVALID_TYPE 0x71

namespace glib
{
	typedef int16_t ErrorCode;
	typedef std::function<void(ErrorCode, const std::string&, const std::string&, const std::string&)> ErrorCallback;

	class Error
	{
	public:
		GLIB_API static void Throw(ErrorCode code, const std::string& message = "");
		GLIB_API static void Throw(ErrorCode code, const std::string& message, const std::string& clazz, const std::string& func);
		GLIB_API static ErrorCode GetLastErrorCode(bool reset = true);
		GLIB_API static const std::string& GetLastErrorMessage(bool reset = true);
		GLIB_API static void SetCallback(ErrorCallback callback);
	};
}