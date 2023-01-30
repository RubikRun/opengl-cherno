#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource parseShader(const std::string& filepath) {
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

static GLuint compileShader(const std::string& sourceStr, GLenum type) {
    const GLuint id = glCreateShader(type);
    const GLchar* sourceData = sourceStr.c_str();
    glShaderSource(id, 1, &sourceData, nullptr);
    glCompileShader(id);

    GLint compileStatus;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        int msgLength;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &msgLength);
        GLchar* message = new GLchar[msgLength];
        glGetShaderInfoLog(id, msgLength, &msgLength, message);
        if (type == GL_VERTEX_SHADER) {
            std::cout << "Failed to compile vertex shader: ";
        }
        else {
            std::cout << "Failed to compile fragment shader: ";
        }
        std::cout << message << std::endl;
        delete[] message;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static GLuint createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    const GLuint program = glCreateProgram();
    const GLuint vertShad = compileShader(vertexShader, GL_VERTEX_SHADER);
    const GLuint fragShad = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vertShad);
    glAttachShader(program, fragShad);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertShad);
    glDeleteShader(fragShad);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);

    ShaderProgramSource shaderSource = parseShader("res/shaders/Basic.shader");
    std::cout << "VERTEX" << std::endl;
    std::cout << shaderSource.vertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << shaderSource.fragmentSource << std::endl;

    const GLuint shader = createShader(shaderSource.vertexSource, shaderSource.fragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}