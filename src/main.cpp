#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Math.h>

// Begone, foul fiend!
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

void OnResize(GLFWwindow* window, int width, int height);
void OnInput(GLFWwindow* window);

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

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, OnResize);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Only loading relevant shaders
    GLuint vsTransform = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/Transform.vert");
    GLuint fsColor = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/Color.frag");
    GLuint shaderTransform = CreateProgram(vsTransform, fsColor);

    Mesh cube, monkey;
    CreateMesh(cube, "assets/meshes/cube.obj");
    CreateMesh(monkey, "assets/meshes/monkey.obj");

    // Only one shader so we don't need to bind it for every object, or even every frame
    GLuint shader = shaderTransform;
    glUseProgram(shader);

    // Setup OpenGL state
    glEnable(GL_DEPTH_TEST);// Disabled by default
    glEnable(GL_CULL_FACE); // Disabled by default
    glCullFace(GL_BACK);    // GL_BACK by default
    glFrontFace(GL_CCW);    // GL_CCW by default

    float prev = glfwGetTime();
    float curr = prev;

    while (!glfwWindowShouldClose(window))
    {
        float dt = curr - prev, tt = glfwGetTime();
        prev = curr;
        curr = tt;

        OnInput(window);

        Color bg{ 0.0f, 0.0f, 0.0f, 1.0f };
        Color tint{ 1.0f, 1.0f, 1.0f, 1.0f };

        glClearColor(bg.r, bg.g, bg.b, bg.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLint uColor = glGetUniformLocation(shader, "u_color");
        GLint uTransform = glGetUniformLocation(shader, "u_transform");
        
        Matrix scale = MatrixIdentity();
        Matrix rotation = MatrixIdentity();
        Matrix translation = MatrixIdentity();
        Matrix model = MatrixIdentity();//scale * rotation * translation;
        const Matrix view = LookAt({ 0.0f, 0.0f, 15.0f }, {}, { 0.0f, 1.0f, 0.0f });
        const Matrix proj = Perspective(60.0f * DEG2RAD, (float)SCR_WIDTH / (float)(SCR_HEIGHT), 0.001f, 1000.0f);
        Matrix mvp = MatrixIdentity();//model * view * proj;

        // Cube 1:
        //model = Translate(-1.0f, 0.0, 1.0f);
        //mvp = model * view * proj;
        //glUniformMatrix4fv(uTransform, 1, GL_TRUE, &mvp.m0);
        //glUniform3f(uColor, 1.0f, 0.0f, 0.0f);
        //glBindVertexArray(cube.vao);
        //glDrawArrays(GL_TRIANGLES, 0, cube.vertexCount);
        //
        //// Cube 2:
        //model = Translate(0.0f, 0.0f, 0.0f);
        //mvp = model * view * proj;
        //glUniformMatrix4fv(uTransform, 1, GL_TRUE, &mvp.m0);
        //glUniform3f(uColor, 0.0f, 1.0f, 0.0f);
        //glBindVertexArray(cube.vao);
        //glDrawArrays(GL_TRIANGLES, 0, cube.vertexCount);
        //
        //// Cube 3:
        //model  = Translate(1.0f, 0.0f, -1.0f);
        //mvp = model * view * proj;
        //glUniformMatrix4fv(uTransform, 1, GL_TRUE, &mvp.m0);
        //glUniform3f(uColor, 0.0f, 0.0f, 1.0f);
        //glBindVertexArray(cube.vao);
        //glDrawArrays(GL_TRIANGLES, 0, cube.vertexCount);

        // Suzane (monkey)
        model = Scale(5.0f, 5.0f, 5.0f);
        mvp = model * view * proj;
        glUniformMatrix4fv(uTransform, 1, GL_TRUE, &mvp.m0);
        glUniform3f(uColor, 1.0f, 1.0f, 1.0f);
        glBindVertexArray(monkey.vao);
        glClear(GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, monkey.vertexCount);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    DestroyMesh(monkey);
    DestroyMesh(cube);
    glDeleteProgram(shaderTransform);

    glfwTerminate();
    return 0;
}

void OnInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void OnResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
