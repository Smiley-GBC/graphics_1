#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Math.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Mesh.h"
#include "Lines.h"
#include "Shader.h"

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
void OnResize(GLFWwindow* window, int width, int height);
void OnInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

GLuint CreateTexture(const char* path)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (data)
    {
        assert(channels == 3 || channels == 4);
        GLenum format = channels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        assert(false, "Texture load failed");
    }
    stbi_image_free(data);
    return texture;
}

int main(const char* path)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

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

    // Enable OpenGL debug context if context allows for debug context
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    Mesh van, plane, cube1x1, ct4, cybertruck;// , airplane;
    CreateMesh(van, "assets/meshes/van.obj");
    CreateMesh(ct4, "assets/meshes/ct4.obj");
    CreateMesh(cybertruck, "assets/meshes/cybertruck.obj");
    //CreateMesh(airplane, "assets/meshes/airplane.obj");
    CreateMesh(plane, "assets/meshes/plane_xz_1x1.obj");
    CreateMesh(cube1x1, "assets/meshes/cube_1x1.obj");

    //GLuint airplaneTex = CreateTexture("assets/textures/airplane.png");
    GLuint vanTex = CreateTexture("assets/textures/van.png");
    GLuint ct4Textures[4] =
    {
        CreateTexture("assets/textures/ct4_red.png"),
        CreateTexture("assets/textures/ct4_orange.png"),
        CreateTexture("assets/textures/ct4_blue.png"),
        CreateTexture("assets/textures/ct4_black.png")
    };
    int textureIndexA = 0;
    int textureIndexB = 0;

    GLuint vsDefault = CreateShader(GL_VERTEX_SHADER, "assets/shaders/default.vert");
    GLuint vsFsq = CreateShader(GL_VERTEX_SHADER, "assets/shaders/fsq.vert");
    GLuint fsColor = CreateShader(GL_FRAGMENT_SHADER, "assets/shaders/color.frag");
    GLuint fsPhong = CreateShader(GL_FRAGMENT_SHADER, "assets/shaders/phong.frag");
    GLuint fsTexture = CreateShader(GL_FRAGMENT_SHADER, "assets/shaders/texture.frag");
    GLuint fsTextureMix = CreateShader(GL_FRAGMENT_SHADER, "assets/shaders/texture_mix.frag");
    GLuint fsNormals = CreateShader(GL_FRAGMENT_SHADER, "assets/shaders/normals.frag");
    GLuint fsGradient = CreateShader(GL_FRAGMENT_SHADER, "assets/shaders/gradient.frag");
    GLuint fsFractal = CreateShader(GL_FRAGMENT_SHADER, "assets/shaders/fractal.frag");

    GLuint shaderColor = CreateProgram(vsDefault, fsColor);
    GLuint shaderPhong = CreateProgram(vsDefault, fsPhong);
    GLuint shaderTexture = CreateProgram(vsDefault, fsTexture);
    GLuint shaderTextureMix = CreateProgram(vsDefault, fsTextureMix);
    GLuint shaderNormals = CreateProgram(vsDefault, fsNormals);
    GLuint shaderGradient = CreateProgram(vsFsq, fsGradient);
    GLuint shaderFractal = CreateProgram(vsFsq, fsFractal);

    Vector3 cameraPosition = Vector3{ 1.0f, 1.0f, 1.0f } * 50.0f;
    float cameraPitch = 25.0f * DEG2RAD;
    float cameraYaw = 180.0f * DEG2RAD;
    float cameraLinearSpeed = 100.0f;
    float cameraAngularSpeed = 250.0f * DEG2RAD;

    Vector3 vanTranslation{};
    float vanRotationX = 0.0f * DEG2RAD;
    float vanRotationY = 0.0f * DEG2RAD;
    float vanSpeed = 100.0f;
    float vanRotationSpeed = 250.0f * DEG2RAD;
    Vector3 vanColor{ 1.0f, 1.0f, 1.0f };

    Vector3 lightColor{ 1.0f, 1.0f, 1.0f };
    float ambientStrength = 1.0f;

    float prev = glfwGetTime();
    float curr = prev;

    GLuint vaoFsq;
    glGenVertexArrays(1, &vaoFsq);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        float dt = curr - prev, tt = glfwGetTime();
        prev = curr;
        curr = tt;
        float vanTranslationDelta = vanSpeed * dt;
        float vanRotationDelta = vanRotationSpeed * dt;
        float cameraTranslationDelta = cameraLinearSpeed * dt;
        float cameraRotationDelta = cameraAngularSpeed * dt;
        OnInput(window);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            Vector2 center{ SCR_WIDTH * 0.5f, SCR_HEIGHT * 0.5f };
            Vector2 delta = Normalize(Vector2{ (float)x - center.x, (float)y - center.y });
            cameraPitch += delta.y * cameraRotationDelta;
            cameraYaw -= delta.x * cameraRotationDelta;
            glfwSetCursorPos(window, center.x, center.y);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        Matrix cameraOrientation = ToMatrix(FromEuler(cameraPitch, cameraYaw, 0.0f));
        Vector3 cameraForward = Forward(cameraOrientation);
        Vector3 cameraRight = Right(cameraOrientation);
        Vector3 cameraUp = Up(cameraOrientation);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPosition = cameraPosition + cameraForward * cameraTranslationDelta;
        }

        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPosition = cameraPosition - cameraForward * cameraTranslationDelta;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPosition = cameraPosition + cameraRight * cameraTranslationDelta;
        }

        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPosition = cameraPosition - cameraRight * cameraTranslationDelta;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            cameraPosition = cameraPosition + cameraUp * cameraTranslationDelta;
        }

        else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            cameraPosition = cameraPosition - cameraUp * cameraTranslationDelta;
        }

        Matrix translation = MatrixIdentity();
        Matrix rotation = MatrixIdentity();
        Matrix scale = MatrixIdentity();
        Matrix proj = Perspective(60.0f * DEG2RAD, SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 1000.0f);
        Matrix view = LookAt(cameraPosition, cameraPosition + cameraForward, { 0.0f, 1.0f, 0.0f });
        Matrix model = scale * rotation * translation;
        Matrix mvp = model * view * proj;
        
        GLuint shader = GL_NONE;
        GLint uTime = GL_NONE;
        GLint uColor = GL_NONE;
        GLint uTexture = GL_NONE;
        GLint uResolution = GL_NONE;
        GLint uTransform = GL_NONE;

        glClearColor(0.25f, 0.75f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw fsq
        glDepthMask(GL_FALSE);
        glUseProgram(shaderFractal);
        uTime = glGetUniformLocation(shaderFractal, "u_time");
        uResolution = glGetUniformLocation(shaderFractal, "u_resolution");
        glUniform1f(uTime, tt);
        glUniform2f(uResolution, SCR_WIDTH, SCR_HEIGHT);
        glBindVertexArray(vaoFsq);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDepthMask(GL_TRUE);

        // Translate van
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        {
            vanRotationY += vanRotationDelta;
        }
        else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        {
            vanRotationY -= vanRotationDelta;
        }
        
        // Rotate van
        Matrix vanRotation = RotateY(vanRotationY) * RotateX(vanRotationX);
        Vector3 vanForward = Forward(vanRotation);
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            vanTranslation = vanTranslation + vanForward * vanTranslationDelta;
        }
        else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        {
            vanTranslation = vanTranslation - vanForward * vanTranslationDelta;
        }

        // Illuminated CT4
        model = vanRotation * Translate(vanTranslation.x, vanTranslation.y, vanTranslation.z);
        mvp = model * view * proj;
        shader = shaderPhong;
        GLint uObjectColor = glGetUniformLocation(shader, "u_object_color");
        GLint uLightColor = glGetUniformLocation(shader, "u_light_color");
        GLint uAmbientStrength = glGetUniformLocation(shader, "u_ambient_strength");
        uTransform = glGetUniformLocation(shader, "u_mvp");
        glUseProgram(shader);
        glUniform3fv(uObjectColor, 1, (float*)&vanColor);
        glUniform3fv(uLightColor, 1, (float*)&lightColor);
        glUniform1f(uAmbientStrength, ambientStrength);
        glUniformMatrix4fv(uTransform, 1, GL_TRUE, &mvp.m0);
        glBindVertexArray(van.vao);
        glDrawArrays(GL_TRIANGLES, 0, van.vertexCount);

        // Draw local axes
        float lineLength = 7.5f;
        Vector3 forward = Forward(vanRotation) * lineLength + vanTranslation;
        Vector3 right = Right(vanRotation) * lineLength + vanTranslation;
        Vector3 up = Up(vanRotation) * lineLength + vanTranslation;
        DrawLine(vanTranslation, forward, view, proj, cube1x1, shaderColor, BLUE, 0.1f, 0.1f);
        DrawLine(vanTranslation, right, view, proj, cube1x1, shaderColor, RED, 0.1f, 0.1f);
        DrawLine(vanTranslation, up, view, proj, cube1x1, shaderColor, GREEN, 0.1f, 0.1f);

        // Draw plane
        mvp = Scale(100.0f, 1.0f, 100.0f) * view * proj;
        shader = shaderColor;
        uColor = glGetUniformLocation(shader, "u_color");
        uTransform = glGetUniformLocation(shader, "u_mvp");
        glUseProgram(shader);
        glUniform3f(uColor, 0.375f, 0.375f, 0.375f);
        glUniformMatrix4fv(uTransform, 1, GL_TRUE, &mvp.m0);
        glBindVertexArray(plane.vao);
        glDrawArrays(GL_TRIANGLES, 0, plane.vertexCount);

        // Draw UI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SliderFloat3("Camera Position", (float*)&cameraPosition, -100.0f, 100.0f);
        ImGui::SliderAngle("Camera Pitch", &cameraPitch);
        ImGui::SliderAngle("Camera Yaw", &cameraYaw);

        ImGui::Separator();
        ImGui::SliderAngle("Van Yaw", &vanRotationY);
        ImGui::ColorPicker3("Van Colour", (float*)&vanColor);
        ImGui::ColorPicker3("Light Colour", (float*)&lightColor);
        ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 1.0f);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}