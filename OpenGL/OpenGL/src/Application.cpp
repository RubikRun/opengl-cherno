#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>
#include <ctime>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <cstdlib>

// Seeds the rand() function with the current time
void setupRand() {
    const unsigned timeNow = unsigned(time(nullptr));
    srand(timeNow);
}

// Returns a random float in given bounds [a, b]
float getRandFloat(float a, float b) {
    float rZeroOne = float(rand()) / float(RAND_MAX);
    return a + (b - a) * rZeroOne;
}

// Returns a random integer in given bounds [a, b]
int getRandInt(int a, int b) {
    return a + rand() % (b - a + 1);
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
    setupRand();

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
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

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * (2 + 3), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (2 + 3), (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    const std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "layout(location = 1) in vec4 color;\n"
        "out vec4 vColor;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "   vColor = color;\n"
        "}\n"
        ;

    const std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "in vec4 vColor;"
        "layout(location = 0) out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vColor;\n"
        "}\n"
        ;

    const GLuint shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    const float aspect = 1920.0f / 1080.0f;
    const float sqSide = 0.05f;
    float sqShape[12] = {
        -sqSide, -sqSide * aspect,
        -sqSide,  sqSide* aspect,
         sqSide, -sqSide * aspect,
        -sqSide,  sqSide* aspect,
         sqSide,  sqSide* aspect,
         sqSide, -sqSide * aspect
    };

    const int sqCount = 1000;

    float bufferData[sqCount * (2 + 3) * 6];
    for (int i = 0; i < sqCount; i++) {
        const float xCenter = getRandFloat(-0.3f, 0.3f);
        const float yCenter = getRandFloat(-0.3f, 0.3f);

        const float rColor = getRandFloat(0.0f, 1.0f);
        const float gColor = getRandFloat(0.0f, 1.0f);
        const float bColor = getRandFloat(0.0f, 1.0f);
        for (int j = 0; j < 6; j++) {
            bufferData[i * (2 + 3) * 6 + j * (2 + 3) + 0] = sqShape[j * 2 + 0] + xCenter;
            bufferData[i * (2 + 3) * 6 + j * (2 + 3) + 1] = sqShape[j * 2 + 1] + yCenter;
            bufferData[i * (2 + 3) * 6 + j * (2 + 3) + 2] = rColor;
            bufferData[i * (2 + 3) * 6 + j * (2 + 3) + 3] = gColor;
            bufferData[i * (2 + 3) * 6 + j * (2 + 3) + 4] = bColor;
        }
    }

    const float maxSpeed = 0.002f;

    float sqVel[sqCount * 2];
    for (int i = 0; i < sqCount; i++) {
        sqVel[i * 2 + 0] = getRandFloat(-maxSpeed, maxSpeed);
        sqVel[i * 2 + 1] = getRandFloat(-maxSpeed * aspect, maxSpeed * aspect);
    }

    const float gravity = 0.0003f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3 * sqCount);

        for (int i = 0; i < sqCount; i++) {
            const float xCenterOld = bufferData[i * (2 + 3) * 6 + 0] - sqShape[0];
            const float yCenterOld = bufferData[i * (2 + 3) * 6 + 1] - sqShape[1];
            const float xCenter = xCenterOld + sqVel[i * 2 + 0];
            const float yCenter = yCenterOld + sqVel[i * 2 + 1];

            for (int j = 0; j < 6; j++) {
                bufferData[i * (2 + 3) * 6 + j * (2 + 3) + 0] = sqShape[j * 2 + 0] + xCenter;
                bufferData[i * (2 + 3) * 6 + j * (2 + 3) + 1] = sqShape[j * 2 + 1] + yCenter;
            }

            sqVel[i * 2 + 0] = sqVel[i * 2 + 0] * (1.0f - gravity) + (-xCenter * maxSpeed) * gravity;
            sqVel[i * 2 + 1] = sqVel[i * 2 + 1] * (1.0f - gravity) + (-yCenter * maxSpeed) * gravity;
        }
        glBufferData(GL_ARRAY_BUFFER, (2 + 3) * 6 * sqCount, bufferData, GL_STATIC_DRAW);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        //Sleep(1);
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}