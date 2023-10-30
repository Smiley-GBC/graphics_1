#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Math.h>

#include "Mesh.h"
#include "Shader.h"
#include "Physics.h"
#include <iostream>

#define GRAVITY { 0.0f, -9.8f, 0.0f }

void OnResize(GLFWwindow* window, int width, int height);
void OnInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
    GLuint fsLighting = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/Lighting.frag");
    GLuint fsDefault = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/Default.frag");
    GLuint fsColor = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/Color.frag");
    GLuint shaderTransform = CreateProgram(vsTransform, fsColor);

    Entity e;
    e.pos = { 2.5, 0.0f, 0.0f };
    Entity& test = Add(e);
    for (float i = -15.0f; i < 15.0f; i += 5.0f)
    {
        Entity entity;
        entity.pos = { i, 50.0f, 0.0f };
        Add(entity);
        printf("%f %f %f\n", test.pos.x, test.pos.y, test.pos.z);
    }

    Mesh cube, sphere, plane, monkey;
    CreateMesh(cube, "assets/meshes/cube.obj");
    CreateMesh(sphere, "assets/meshes/sphere.obj");
    CreateMesh(plane, "assets/meshes/plane.obj");
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
        //Simulate(test, GRAVITY, dt);
        SimulateAll(GRAVITY, dt);

        glClearColor(0.0f, 0.75f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLint uColor = glGetUniformLocation(shader, "u_color");
        GLint uTransform = glGetUniformLocation(shader, "u_transform");
        
        Matrix scale = MatrixIdentity();
        Matrix rotation = MatrixIdentity();
        Matrix translation = MatrixIdentity();
        Matrix model = MatrixIdentity();//scale * rotation * translation;
        const Matrix view = LookAt({ 50.0f, 50.0f, 50.0f }, {}, { 0.0f, 1.0f, 0.0f });
        const Matrix proj = Perspective(60.0f * DEG2RAD, (float)SCR_WIDTH / (float)(SCR_HEIGHT), 0.001f, 1000.0f);
        Matrix mvp = MatrixIdentity();//model * view * proj;

        // Ground Plane
        model = Scale(100.0f, 1.0f, 100.0f);
        mvp = model * view * proj;
        glUniformMatrix4fv(uTransform, 1, GL_TRUE, &mvp.m0);
        glUniform3f(uColor, 0.375f, 0.375f, 0.375f);
        glBindVertexArray(plane.vao);
        glDrawArrays(GL_TRIANGLES, 0, plane.vertexCount);

        // Entities
        for (const Entity& entity : All())
        {
            model = Translate(entity.pos.x, entity.pos.y, entity.pos.z);
            mvp = model * view * proj;
            glUniformMatrix4fv(uTransform, 1, GL_TRUE, &mvp.m0);
            glUniform3f(uColor, 1.0f, 1.0f, 1.0f);
            glBindVertexArray(sphere.vao);
            glDrawArrays(GL_TRIANGLES, 0, sphere.vertexCount);
        }

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
