#pragma once

#include "../DLLDefs.h"

#include <string>

namespace glib
{
	class FileUtils
	{
	public:
		FileUtils() = delete;
		~FileUtils() = delete;

		GLIB_API static std::string ReadFileToString(const std::string& path);
	};
}