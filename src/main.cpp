#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Math.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Mesh.h"
#include "Shader.h"
#include <iostream>
#include "Physics.h"

void OnResize(GLFWwindow* window, int width, int height);
void OnInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
#define GRAVITY {0.0f, -9.8f, 0.0f}

int main()
{
    Ids entities(6);
    size_t counter = 0;
    for (float i = -15.0f; i < 15.0f; i += 5.0f)
        entities[counter++] = Add({ i, 50.0f, 0.0f });

    Entities& e = All();
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

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    
    // Only loading relevant shaders
    GLuint vsTransform = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/Transform.vert");
    GLuint fsLighting = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/Lighting.frag");
    GLuint fsColor = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/Color.frag");
    GLuint shaderLighting = CreateProgram(vsTransform, fsLighting);
    GLuint shaderColor = CreateProgram(vsTransform, fsColor);

    Mesh cube, plane, sphere, monkey;
    CreateMesh(cube, "assets/meshes/cube.obj");
    CreateMesh(plane, "assets/meshes/plane.obj");
    CreateMesh(sphere, "assets/meshes/sphere.obj");
    CreateMesh(monkey, "assets/meshes/monkey.obj");

    // Only one shader so we don't need to bind it for every object, or even every frame
    GLuint shader = shaderColor;
    glUseProgram(shader);

    // Setup OpenGL state
    glEnable(GL_DEPTH_TEST);// Disabled by default
    glEnable(GL_CULL_FACE); // Disabled by default
    glCullFace(GL_BACK);    // GL_BACK by default
    glFrontFace(GL_CCW);    // GL_CCW by default

    float prev = glfwGetTime();
    float curr = prev;
    Vector3 cameraPosition{ 50.0f, 50.0f, 50.0f };
    while (!glfwWindowShouldClose(window))
    {
        float dt = curr - prev, tt = glfwGetTime();
        prev = curr;
        curr = tt;

        OnInput(window);
        SimulateAll(GRAVITY, dt);

        glClearColor(0.0f, 0.75f, 0.90f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLint uColor = GL_NONE;
        GLint uTransform = GL_NONE;
        
        Matrix scale = MatrixIdentity();
        Matrix rotation = MatrixIdentity();
        Matrix translation = MatrixIdentity();
        Matrix model = MatrixIdentity();//scale * rotation * translation;
        const Matrix view = LookAt(cameraPosition, {}, { 0.0f, 1.0f, 0.0f });
        const Matrix proj = Perspective(60.0f * DEG2RAD, (float)SCR_WIDTH / (float)(SCR_HEIGHT), 0.001f, 1000.0f);
        Matrix mvp = MatrixIdentity();//model * view * proj;

        // Sphere
        for (const Entity& entity : All())
        {
            model = Translate(entity.pos.x, entity.pos.y, entity.pos.z);
            mvp = model * view * proj;
            shader = shaderLighting;
            uColor = glGetUniformLocation(shader, "u_color");
            uTransform = glGetUniformLocation(shader, "u_transform");
            glUseProgram(shader);
            glUniformMatrix4fv(uTransform, 1, GL_TRUE, &mvp.m0);
            glUniform3f(uColor, 1.0f, 0.0f, 0.0f);
            glBindVertexArray(sphere.vao);
            glDrawArrays(GL_TRIANGLES, 0, sphere.vertexCount);
        }

        // Plane
        model = Scale(100.0f, 1.0f, 100.0f);
        mvp = model * view * proj;
        shader = shaderColor;
        uColor = glGetUniformLocation(shader, "u_color");
        uTransform = glGetUniformLocation(shader, "u_transform");
        glUseProgram(shader);
        glUniformMatrix4fv(uTransform, 1, GL_TRUE, &mvp.m0);
        glUniform3f(uColor, 0.375f, 0.375f, 0.375f);
        glBindVertexArray(plane.vao);
        glDrawArrays(GL_TRIANGLES, 0, plane.vertexCount);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        ImGui::SliderFloat3("Camera Position", &cameraPosition.x, -100.0f, 100.0f);
        //ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    DestroyMesh(monkey);
    DestroyMesh(cube);
    glDeleteProgram(shaderLighting);

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
