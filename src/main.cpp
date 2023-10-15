#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Math.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
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

using Vertices = std::vector<Vector3>;

struct TernaryTree
{

};

Vertices Aids(Vector3 origin, float length)
{
    Vertices vertices(3);

    Vector3 up = Normalize(Vector3{ 0.0f, 1.0f, 0.0f });
    Vector3 left = Normalize(Vector3{ -0.866035879f, -0.5f, 0.0f });
    Vector3 right = Normalize(Vector3{ 0.866035879f, -0.5f, 0.0f });

    vertices[0] = origin + up * length;
    vertices[1] = origin + left * length;
    vertices[2] = origin + right * length;

    return vertices;
}

void Aids3(Vector3 origin, Vertices& vertices, float length, size_t iterations)
{
    if (iterations > 0)
    {
        for (size_t i = 0; i < vertices.size(); i++)
        {
            // Something like this, but with the correct offset into vertices
            Aids3(vertices[i], vertices, length * 0.5f, iterations - 1);
        }
    }
}

//Vertices Create(Vector3 origin, Vector3 up, Vector3 left, Vector3 right, float length)
//{
//    Vertices vertices(6);
//    vertices[0] = vertices[2] = vertices[4] = origin;
//    vertices[1] = origin + up * length;
//    vertices[3] = origin + left * length;
//    vertices[5] = origin + right * length;
//    return vertices;
//}
//
//std::vector<Vertices> Create3(const Vertices& positions, const Vertices& directions, float length)
//{
//    for ()
//}

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
    GLuint shaderDefault = CreateProgram(vsDefault, fsDefault);
    glUseProgram(shaderDefault);

    float length = 1.0f;
    Vector3 up = Normalize(Vector3{ 0.0f, 1.0f, 0.0f });
    Vector3 left = Normalize(Vector3{ -0.866035879f, -0.5f, 0.0f });
    Vector3 right = Normalize(Vector3{ 0.866035879f, -0.5f, 0.0f });
    Vertices vertices = Create({}, up, left, right, 1.0f);

    const size_t iterations = 5;
    std::vector<Vertices> collection(8);
    collection[0] = vertices;
    size_t vertexCount = 3;
    for (size_t i = 1; i < iterations; i++)
    {
        for (size_t j = 0; j < collection[i - 1].size(); j++)
        {
            // this is hurting my brain. The proper way to do this 
        }
        //for (size_t j = 0; j < 6; j+= 2)
        //{
        //    collection[i] = Create(collection[i - 1][j], up, left, right, length);
        //    vertexCount += 6;
        //}
    }

    //float colors[] = {
    //    1.0f, 1.0f, 1.0f,   // white
    //    1.0f, 1.0f, 1.0f    // white
    //};

    GLuint vaoLines;
    GLuint vboLinePositions, vboLineColors;
    glGenVertexArrays(1, &vaoLines);
    glGenBuffers(1, &vboLinePositions);
    glGenBuffers(1, &vboLineColors);

    glBindBuffer(GL_ARRAY_BUFFER, vboLinePositions);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof Vector3, vertices.data(), GL_STATIC_DRAW);

    //glBindBuffer(GL_ARRAY_BUFFER, vboLineColors);
    //glBufferData(GL_ARRAY_BUFFER, sizeof colors, colors, GL_STATIC_DRAW);

    glBindVertexArray(vaoLines);

    glBindBuffer(GL_ARRAY_BUFFER, vboLinePositions);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //glBindBuffer(GL_ARRAY_BUFFER, vboLineColors);
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

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
        glDrawArrays(GL_LINES, 0, 6);

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
