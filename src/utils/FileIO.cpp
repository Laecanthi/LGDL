#include <LGDL/FileIO.h>
#include <LGDL/LGDLCore.h>

#include <fstream>
#include <sstream>
#include <iostream>

namespace LGDL
{
    std::string LoadTextFile(const std::string& relativePath)
    {
        std::string fullPath = ResolvePath(relativePath);

        std::cout << "[LGDL] Attempting to open: "
          << fullPath
          << "\n";

        std::ifstream file(fullPath);

        if (!file.is_open())
        {
            std::cout << "[LGDL] FAILED TO OPEN FILE: " << fullPath << "\n";
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }
}