#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Math.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Mesh.h"
#include "Shader.h"
#include <iostream>

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
    GLuint fsColor = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/Color.frag");
    GLuint shaderTransform = CreateProgram(vsTransform, fsColor);

    Mesh cube, sphere, monkey;
    CreateMesh(cube, "assets/meshes/cube.obj");
    CreateMesh(sphere, "assets/meshes/sphere.obj");
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
    Vector3 cameraPosition{ 50.0f, 50.0f, 50.0f };
    while (!glfwWindowShouldClose(window))
    {
        float dt = curr - prev, tt = glfwGetTime();
        prev = curr;
        curr = tt;

        OnInput(window);

        glClearColor(0.0f, 0.75f, 0.90f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLint uColor = glGetUniformLocation(shader, "u_color");
        GLint uTransform = glGetUniformLocation(shader, "u_transform");
        
        Matrix scale = MatrixIdentity();
        Matrix rotation = MatrixIdentity();
        Matrix translation = MatrixIdentity();
        Matrix model = MatrixIdentity();//scale * rotation * translation;
        const Matrix view = LookAt(cameraPosition, {}, { 0.0f, 1.0f, 0.0f });
        const Matrix proj = Perspective(60.0f * DEG2RAD, (float)SCR_WIDTH / (float)(SCR_HEIGHT), 0.001f, 1000.0f);
        Matrix mvp = MatrixIdentity();//model * view * proj;

        // Suzane (monkey)
        model = Scale(5.0f, 5.0f, 5.0f);
        mvp = model * view * proj;
        glUniformMatrix4fv(uTransform, 1, GL_TRUE, &mvp.m0);
        glUniform3f(uColor, 1.0f, 1.0f, 1.0f);
        glBindVertexArray(sphere.vao);
        glClear(GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, sphere.vertexCount);

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
