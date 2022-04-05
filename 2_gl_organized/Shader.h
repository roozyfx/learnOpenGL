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
//    GLuint id;

private:
    std::string readShaderFile(const std::filesystem::__cxx11::path &path);

private:
    std::string m_vertexFile{};
    std::string m_fragmentFile{};
    GLuint m_shaderProgram;
};
