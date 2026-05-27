#include <LGDL/LGDLCore.h>

#include <glad/glad.h>

namespace LGDL
{
    static std::string g_ResourcePath = "";

    void SetResourcePath(const std::string& path)
    {
        g_ResourcePath = path;
    }

    std::string GetResourcePath()
    {
        return g_ResourcePath;
    }

    std::string ResolvePath(const std::string& relativePath)
    {
        return g_ResourcePath + "/" + relativePath;
    }

    bool Initialize(void* procLoader)
    {
        return gladLoadGLLoader((GLADloadproc)procLoader);
    }

    void SetViewport(int width, int height)
    {
        glViewport(0, 0, width, height);
    }
}