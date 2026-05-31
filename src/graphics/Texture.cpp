#include <LGDL/Texture.h>

#include <stb/stb_image.h>
#include <glad/glad.h>
#include <iostream>
#include <LGDL/LGDL.h>

namespace LGDL
{
    Texture LoadTexture(const std::string& relativePath)
    {
        Texture texture;

        const std::string& path = ResolvePath(relativePath);

        // flip because OpenGL UV origin is bottom-left
        stbi_set_flip_vertically_on_load(true);

        unsigned char* data = stbi_load(
            path.c_str(),
            &texture.width,
            &texture.height,
            &texture.channels,
            0
        );

        if(!data)
        {
            std::cout << "Failed to load texture: "
                    << path << "\n";

            return texture;
        }

        // create texture
        glGenTextures(1, &texture.ID);

        glBindTexture(GL_TEXTURE_2D, texture.ID);

        // texture settings
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE
        );

        GLenum format;

        if(texture.channels == 4)
        {
            format = GL_RGBA;
        }
        else if(texture.channels == 3)
        {
            format = GL_RGB;
        }
        else
        {
            format = GL_RED;
        }

        // upload texture
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            texture.width,
            texture.height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data
        );

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR
        );

        // free CPU image data
        stbi_image_free(data);

        return texture;
    }
}