#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Math.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

void OnResize(GLFWwindow* window, int width, int height);
void OnInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct Color
{
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
};

GLuint CreateShader(GLint type, const char* path)
{
    GLuint shader = 0;
    try
    {
        // Load text file
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        file.open(path);

        // Interpret the file as a giant string
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();

        // Verify shader type matches shader file extension
        const char* ext = strrchr(path, '.');
        switch (type)
        {
        case GL_VERTEX_SHADER:
            assert(strcmp(ext, ".vert") == 0);
            break;

        case GL_FRAGMENT_SHADER:
            assert(strcmp(ext, ".frag") == 0);
            break;
        default:
            assert(false, "Invalid shader type");
            break;
        }

        // Compile text as a shader
        std::string str = stream.str();
        const char* src = str.c_str();
        shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);

        // Check for compilation errors
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "Shader failed to compile: \n" << infoLog << std::endl;
        }
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "Shader (" << path << ") not found: " << e.what() << std::endl;
        // Alternatively, you can replace the above line with "throw;" to crash your program if a shader fails to load
        //throw;
    }
    return shader;
}

GLuint CreateProgram(GLuint vs, GLuint fs)
{
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        shaderProgram = GL_NONE;
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    return shaderProgram;
}

enum State
{
    OBJ_1,
    OBJ_2,
    OBJ_3,
    OBJ_4,
    OBJ_5,
} state;

struct Vertex
{
    Vector3 position;
    Vector3 color;
};

struct Square
{
    Vertex vertices[4];
};

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, OnResize);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    GLuint vsDefault = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/Default.vert");
    GLuint fsDefault = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/Default.frag");

    GLuint vsAnimate = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/Animate.vert");
    GLuint vsTransform = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/Transform.vert");

    GLuint fsColor = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/Color.frag");
    GLuint fsColorFade = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/ColorFaded.frag");

    GLuint shaderDefault = CreateProgram(vsDefault, fsDefault);
    GLuint shaderDefaultColor = CreateProgram(vsDefault, fsColor);
    GLuint shaderDefaultFade = CreateProgram(vsDefault, fsColorFade);

    GLuint shaderAnimate = CreateProgram(vsAnimate, fsColor);
    GLuint shaderTransform = CreateProgram(vsTransform, fsColor);
    glUseProgram(shaderDefault);

    Square square;
    square.vertices[0].position = { -1.0f, 1.0f, 0.0f };
    square.vertices[1].position = {  1.0f, 1.0f, 0.0f };
    square.vertices[2].position = {  1.0f, -1.0f, 0.0f };
    square.vertices[3].position = { -1.0f, -1.0f, 0.0f };
    square.vertices[0].color = { 1.0f, 1.0f, 1.0f };
    square.vertices[1].color = { 1.0f, 0.0f, 0.0f };
    square.vertices[2].color = { 0.0f, 1.0f, 0.0f };
    square.vertices[3].color = { 0.0f, 0.0f, 1.0f };
    //for (int i = 0; i < 4; i++)
    //    square.vertices[i].color = { 1.0f, 1.0f, 1.0f };

    GLuint vaoLines;
    GLuint vboLines;
    glGenVertexArrays(1, &vaoLines);
    glGenBuffers(1, &vboLines);
    
    glBindVertexArray(vaoLines);
    glBindBuffer(GL_ARRAY_BUFFER, vboLines);
    glBufferData(GL_ARRAY_BUFFER, sizeof Square, &square, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glLineWidth(25.0f);
    float prev = glfwGetTime();
    float curr = prev;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float dt = curr - prev, tt = glfwGetTime();
        prev = curr;
        curr = tt;

        // input
        // -----
        OnInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);        
        glDrawArrays(GL_LINES, 0, 4);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void OnInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        state = OBJ_1;

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        state = OBJ_2;

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        state = OBJ_3;

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        state = OBJ_4;

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        state = OBJ_5;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void OnResize(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
