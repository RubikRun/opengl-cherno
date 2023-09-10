#include "Shader.h"

#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath)
	: filepath(filepath)
{
    ShaderProgramSource shaderSource = parseShader(filepath);
    rendererId = createShader(shaderSource.vertexSource, shaderSource.fragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(rendererId));
}

void Shader::bind() const {
    GLCall(glUseProgram(rendererId));
}
void Shader::unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int v) {
    const int location = getUniformLocation(name);
    GLCall(glUniform1i(location, v));
}


void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    const int location = getUniformLocation(name);
    GLCall(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    const int location = getUniformLocation(name);
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}

int Shader::getUniformLocation(const std::string& name) {
    if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
        return uniformLocationCache[name];
    }
    GLCall(const int location = glGetUniformLocation(rendererId, name.c_str()));
    if (location < 0) {
        std::cout << "WARNING: Uniform " << name << " doesn't exist." << std::endl;
    }
    uniformLocationCache[name] = location;
    return location;
}

ShaderProgramSource Shader::parseShader(const std::string& filepath) const {
    std::ifstream file(filepath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    } shaderType = ShaderType::NONE;

    std::string line;
    std::stringstream vertexStream;
    std::stringstream fragmentStream;
    while (getline(file, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                shaderType = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                shaderType = ShaderType::FRAGMENT;
            }
            else {
                std::cout << "WARNING: #shader does not specify a valid shader type." << std::endl;
            }
        }
        else {
            if (shaderType == ShaderType::VERTEX) {
                vertexStream << line << std::endl;
            }
            else if (shaderType == ShaderType::FRAGMENT) {
                fragmentStream << line << std::endl;
            }
            else {
                std::cout << "WARNING: #shader type not specified in the beginning." << std::endl;
            }
        }
    }

    return { vertexStream.str(), fragmentStream.str() };
}

GLuint Shader::compileShader(const std::string& sourceStr, GLenum type) const {
    GLCall(const GLuint id = glCreateShader(type));
    const GLchar* sourceData = sourceStr.c_str();
    GLCall(glShaderSource(id, 1, &sourceData, nullptr));
    GLCall(glCompileShader(id));

    GLint compileStatus;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus));
    if (compileStatus == GL_FALSE) {
        int msgLength;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &msgLength));
        GLchar* message = new GLchar[msgLength];
        GLCall(glGetShaderInfoLog(id, msgLength, &msgLength, message));
        if (type == GL_VERTEX_SHADER) {
            std::cout << "Failed to compile vertex shader: ";
        }
        else {
            std::cout << "Failed to compile fragment shader: ";
        }
        std::cout << message << std::endl;
        delete[] message;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

GLuint Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) const {
    GLCall(const GLuint program = glCreateProgram());
    const GLuint vertShad = compileShader(vertexShader, GL_VERTEX_SHADER);
    const GLuint fragShad = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

    GLCall(glAttachShader(program, vertShad));
    GLCall(glAttachShader(program, fragShad));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vertShad));
    GLCall(glDeleteShader(fragShad));

    return program;
}