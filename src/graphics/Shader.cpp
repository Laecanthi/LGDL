#include <LGDL/Shader.h>

#include <LGDL/FileIO.h>

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

namespace LGDL
{
    GLuint CreateShaderProgram(
        const char* vertexPath,
        const char* fragmentPath
    )
    {
        // load shader files

        std::string vertexSource = LGDL::LoadTextFile(vertexPath);
        std::string fragmentSource = LGDL::LoadTextFile(fragmentPath);

        // create vertex shader

        GLuint vertexShader =
            glCreateShader(GL_VERTEX_SHADER);

        const char* vertexCString = vertexSource.c_str();

        glShaderSource(
            vertexShader,
            1,
            &vertexCString,
            nullptr
        );

        glCompileShader(vertexShader);

        // create fragment shader

        GLuint fragmentShader =
            glCreateShader(GL_FRAGMENT_SHADER);

        const char* fragmentCString = fragmentSource.c_str();

        glShaderSource(
            fragmentShader,
            1,
            &fragmentCString,
            nullptr
        );

        glCompileShader(fragmentShader);

        // create program

        GLuint shaderProgram =
            glCreateProgram();

        glAttachShader(shaderProgram, vertexShader);

        glAttachShader(shaderProgram, fragmentShader);

        glLinkProgram(shaderProgram);

        // cleanup temp shaders

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // return program
        
        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

        if (!success)
        {
            char log[1024];
            glGetProgramInfoLog(shaderProgram, 1024, NULL, log);
            std::cout << "SHADER LINK ERROR:\n" << log << "\n";
        }

        glCompileShader(vertexShader);

        //GLint success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char log[1024];
            glGetShaderInfoLog(vertexShader, 1024, NULL, log);
            std::cout << "VERTEX SHADER ERROR:\n" << log << "\n";
        }

        return shaderProgram;
    }
}