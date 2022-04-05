#include "Shader.h"
#include <fstream>
#include <string>
#include "log.h"

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile) :
    m_vertexFile{vertexFile},
    m_fragmentFile{fragmentFile}
{
    std::string vertexCode = readShaderFile(vertexFile);
    const GLchar* vertexSource = vertexCode.c_str();
    std::string fragmentCode = readShaderFile(fragmentFile).c_str();
    const GLchar* fragmentSource = fragmentCode.c_str();

    // Create vertex and fragment shaders and get GLuint refrences to them.
    // Then set the source and compile them.
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    // shaders should be part of a shader program.
    // Attach shaders to the program and link it.
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    //  At this point there's no need for the shader objects and the program is enough,
    // so we delete them to clean up.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_shaderProgram);
}

std::string Shader::readShaderFile(const std::filesystem::path& path)
{
    Log log;
    std::ifstream file(path, std::ios::in | std::ios::binary);
    // Obtain the size of the file.
    std::error_code errorCode;
    const auto sz {std::filesystem::file_size(path, errorCode)};
    if (errorCode){
        log<<__func__;
        log<<errorCode.message() + ": " + path.generic_string();
        return "";
    }
    else{
        // Create a buffer.
        std::string result(sz, '\0');

        // Read the whole file into the buffer.
        file.read(result.data(), sz);

        return result;
    }
}
