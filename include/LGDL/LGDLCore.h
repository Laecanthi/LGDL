#pragma once

#include <string>

namespace LGDL
{
    void SetResourcePath(const std::string& path);
    std::string GetResourcePath();

    std::string ResolvePath(const std::string& relativePath);

    bool Initialize(void* procAddressLoader);

    void SetViewport(int width, int height);
}