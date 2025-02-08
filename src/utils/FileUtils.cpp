#include "glib/utils/FileUtils.h"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

std::string glib::FileUtils::ReadFileToString(const std::string& path)
{
    std::ifstream stream(path);
    if (!stream.is_open()) return "";

    std::stringstream buf;
    buf << stream.rdbuf();

    return buf.str();
}
