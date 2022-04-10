#pragma once

#include <glad/glad.h>
#include <string>
#include <filesystem>

class Shader {
public:
    Shader(const std::string &vertexFile = "shaders/default.vert", const std::string &fragmentFile = "shaders/default.frag");
    ~Shader();

    GLuint id() { return m_shaderProgram; }

    void use(){glUseProgram(m_shaderProgram);}

private:
    std::string readShaderFile(const std::filesystem::__cxx11::path &path);

    // Checks if the different Shaders have compiled properly
    void compileErrors(unsigned int shader, const char* type);

private:
    std::string m_vertexFile{};
    std::string m_fragmentFile{};
    GLuint m_shaderProgram;
};
