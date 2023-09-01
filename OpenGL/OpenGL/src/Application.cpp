#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char *function, const char *file, int line) {
    if (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): "
            << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

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

static GLuint createShader(const std::string& vertexShader, const std::string& fragmentShader) {
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

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    float positions[4 * 2] = {
        -0.5f, -0.5f,
        +0.5f, -0.5f,
        +0.5f, +0.5f,
        -0.5f, +0.5f,
    };

    unsigned int indices[2 * 3] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    GLCall(glEnableVertexAttribArray(0));

    GLuint ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    ShaderProgramSource shaderSource = parseShader("res/shaders/Basic.shader");
    const GLuint shader = createShader(shaderSource.vertexSource, shaderSource.fragmentSource);
    GLCall(glUseProgram(shader));

    GLCall(const GLint u_Color = glGetUniformLocation(shader, "u_Color"));
    ASSERT(u_Color != -1);

    float red = 0.0f;
    float redDelta = 0.005f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        /* Set uniforms */
        GLCall(glUniform4f(u_Color, red, 0.3f, 0.8f, 1.0f));
        /* Render here */
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        /* Update uniforms */
        if (red > 1.0f) {
            redDelta *= -1.0f;
        }
        else if (red < 0.0f) {
            redDelta *= -1.0f;
        }
        red += redDelta;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}