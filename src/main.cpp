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
#include "Materials.h"
#include "Lights.h"

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
void OnResize(GLFWwindow* window, int width, int height);
void OnInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

enum Attenuation
{
    LINEAR,
    EXP2,
    EXP3,
    EASE
} attenuation;

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

void SendMaterial(Material material, GLuint shader)
{
    GLint uAmbient = glGetUniformLocation(shader, "u_material.ambient");
    GLint uDiffuse = glGetUniformLocation(shader, "u_material.diffuse");
    GLint uSpecular = glGetUniformLocation(shader, "u_material.specular");
    GLint uShininess = glGetUniformLocation(shader, "u_material.shininess");

    glUniform3fv(uAmbient, 1, (float*)&material.ambient);
    glUniform3fv(uDiffuse, 1, (float*)&material.diffuse);
    glUniform3fv(uSpecular, 1, (float*)&material.specular);
    glUniform1f(uShininess, material.shininess);
}

void SendMaterialTexture(GLuint diffuse, GLuint specular, float shininess, GLuint shader)
{
    GLuint uDiffuse = glGetUniformLocation(shader, "u_diffuse");
    GLuint uSpecular = glGetUniformLocation(shader, "u_specular");
    GLuint uShininess = glGetUniformLocation(shader, "u_shininess");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse);
    glUniform1i(uDiffuse, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular);
    glUniform1i(uSpecular, 1);

    glUniform1f(uShininess, shininess);
}

void SendLight(Light light, GLint shader)
{
    GLint uPosition = glGetUniformLocation(shader, "u_light.position");
    GLint uAmbient = glGetUniformLocation(shader, "u_light.ambient");
    GLint uDiffuse = glGetUniformLocation(shader, "u_light.diffuse");
    GLint uSpecular = glGetUniformLocation(shader, "u_light.specular");
    GLint uRadius = glGetUniformLocation(shader, "u_light.radius");

    GLint uX = glGetUniformLocation(shader, "u_x");
    GLint uX2 = glGetUniformLocation(shader, "u_x2");
    GLint uX3 = glGetUniformLocation(shader, "u_x3");
    GLint uEase = glGetUniformLocation(shader, "u_ease");

    switch (attenuation)
    {
    case LINEAR:
        glUniform1f(uX, 1.0f);
        glUniform1f(uX2, 0.0f);
        glUniform1f(uX3, 0.0f);
        glUniform1f(uEase, 0.0f);
        break;

    case EXP2:
        glUniform1f(uX, 0.0f);
        glUniform1f(uX2, 1.0f);
        glUniform1f(uX3, 0.0f);
        glUniform1f(uEase, 0.0f);
        break;

    case EXP3:
        glUniform1f(uX, 0.0f);
        glUniform1f(uX2, 0.0f);
        glUniform1f(uX3, 1.0f);
        glUniform1f(uEase, 0.0f);
        break;

    case EASE:
        glUniform1f(uX, 0.0f);
        glUniform1f(uX2, 0.0f);
        glUniform1f(uX3, 0.0f);
        glUniform1f(uEase, 1.0f);
        break;
    }

    glUniform3fv(uPosition, 1, (float*)&light.position);
    glUniform3fv(uAmbient, 1, (float*)&light.ambient);
    glUniform3fv(uDiffuse, 1, (float*)&light.diffuse);
    glUniform3fv(uSpecular, 1, (float*)&light.specular);
    glUniform1f(uRadius, light.radius);
}

void SendTransforms(Matrix mvp, Matrix model, Vector3 eye, GLint shader)
{
    GLint uMVP = glGetUniformLocation(shader, "u_mvp");
    GLint uModel = glGetUniformLocation(shader, "u_model");
    GLint uNormal = glGetUniformLocation(shader, "u_normal");
    GLint uEye = glGetUniformLocation(shader, "u_eye");

    Matrix normal = Transpose(Invert(model));
    float mat3[9] =
    {
        normal.m0, normal.m4, normal.m8,
        normal.m1, normal.m5, normal.m9,
        normal.m2, normal.m6, normal.m10
    };

    glUniformMatrix4fv(uMVP, 1, GL_TRUE, &mvp.m0);
    glUniformMatrix4fv(uModel, 1, GL_TRUE, &model.m0);
    glUniformMatrix3fv(uNormal, 1, GL_TRUE, mat3);
    glUniform3fv(uEye, 1, (float*)&eye);
}

enum Effect : int
{
    GOURAUD,
    PHONG,
    PHONG_MAPS
};

int main(const char* path)
{
    CreateMaterials();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    Mesh plane, cube, sphere, monkey, van, ct4;
    CreateMesh(plane, "assets/meshes/plane.obj");
    CreateMesh(cube, "assets/meshes/cube.obj");
    CreateMesh(sphere, "assets/meshes/sphere.obj");
    CreateMesh(ct4, "assets/meshes/ct4.obj");

    GLuint texCt4[5]
    {
        CreateTexture("assets/textures/ct4_black.png"),
        CreateTexture("assets/textures/ct4_grey.png"),
        CreateTexture("assets/textures/ct4_blue.png"),
        CreateTexture("assets/textures/ct4_red.png"),
        CreateTexture("assets/textures/ct4_orange.png"),
    };
    GLuint texCt4Specular = CreateTexture("assets/textures/ct4_specular.png");
    GLuint texWhite = CreateTexture("assets/textures/white.png");

    GLuint vsDefault = CreateShader(GL_VERTEX_SHADER, "assets/shaders/default.vert");
    GLuint vsGouraud = CreateShader(GL_VERTEX_SHADER, "assets/shaders/gouraud.vert");
    GLuint fsColor = CreateShader(GL_FRAGMENT_SHADER, "assets/shaders/color.frag");
    GLuint fsGouraud = CreateShader(GL_FRAGMENT_SHADER, "assets/shaders/gouraud.frag");
    GLuint fsPhong = CreateShader(GL_FRAGMENT_SHADER, "assets/shaders/phong.frag");
    GLuint fsPhongMaps = CreateShader(GL_FRAGMENT_SHADER, "assets/shaders/phong_maps.frag");
    GLuint shaderColor = CreateProgram(vsDefault, fsColor);
    GLuint shaderPhong = CreateProgram(vsDefault, fsPhong);
    GLuint shaderPhongMaps = CreateProgram(vsDefault, fsPhongMaps);
    GLuint shaderGouraud = CreateProgram(vsGouraud, fsGouraud);
    
    GLuint shaders[3]{ shaderGouraud, shaderPhong, shaderPhongMaps };
    Effect shaderIndex = PHONG;
    int lightIndex = 2;
    bool lightVolume = false;

    Light lights[5];
    Vector3 positons[5];
    Material materials[5];
    materials[0] = chrome;
    materials[1] = obsidian;
    materials[2] = turquoise;
    materials[3] = ruby;
    materials[4] = brass;
    for (size_t i = 0; i < 5; i++)
    {
        static float x = -60.0f;
        positons[i] = Vector3{ x, 0.0f, 0.0f };
        x += 30.0f;

        Vector3 lightColor{ 1.0f, 1.0f, 1.0f };
        lights[i].position = positons[i] + Vector3{ 0.0f, 20.0f, 0.0f };
        lights[i].ambient = lightColor;
        lights[i].diffuse = lightColor;
        lights[i].specular = lightColor;
        lights[i].radius = 30.0f;
    }

    Vector3 cameraPosition{ 60.0f, 10.0f, 25.0f };
    float cameraPitch = 15.0f * DEG2RAD;
    float cameraYaw = 180.0f * DEG2RAD;
    float linearSpeed = 50.0f;
    float angularSpeed = 500.0f * DEG2RAD;

    float prev = glfwGetTime();
    float curr = prev;
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        // Input and deltas
        float dt = curr - prev, tt = glfwGetTime();
        prev = curr;
        curr = tt;
        float linearDelta = linearSpeed * dt;
        float angularDelta = angularSpeed * dt;
        OnInput(window);

        // pitch/yaw
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            Vector2 center{ SCR_WIDTH * 0.5f, SCR_HEIGHT * 0.5f };
            Vector2 delta = Normalize(Vector2{ (float)x - center.x, (float)y - center.y });
            cameraPitch += delta.y * angularDelta;
            cameraYaw -= delta.x * angularDelta;
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

        // forward/backward
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPosition = cameraPosition + cameraForward * linearDelta;
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPosition = cameraPosition - cameraForward * linearDelta;
        }

        // left/right
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPosition = cameraPosition + cameraRight * linearDelta;
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPosition = cameraPosition - cameraRight * linearDelta;
        }

        // up/down
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            cameraPosition = cameraPosition + cameraUp * linearDelta;
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            cameraPosition = cameraPosition - cameraUp * linearDelta;
        }
        
        Matrix proj = Perspective(60.0f * DEG2RAD, SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 1000.0f);
        //Matrix proj = Ortho(-100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f);
        Matrix view = LookAt(cameraPosition, cameraPosition + cameraForward, { 0.0f, 1.0f, 0.0f });
        glClearColor(0.25f, 0.75f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        GLuint ct4Shader = shaders[shaderIndex];
        glUseProgram(ct4Shader);
        for (size_t i = 0; i < 5; i++)
        {
            // Cars
            Matrix model = Translate(positons[i].x, positons[i].y, positons[i].z);
            Matrix mvp = model * view * proj;
            SendTransforms(mvp, model, cameraPosition, ct4Shader);
            SendLight(lights[i], ct4Shader);
            if (shaderIndex == PHONG_MAPS)
                SendMaterialTexture(texCt4[i], texCt4Specular, SPECULAR_POWER, ct4Shader);
            else
                SendMaterial(materials[i], ct4Shader);
            glBindVertexArray(ct4.vao);
            glDrawArrays(GL_TRIANGLES, 0, ct4.vertexCount);
        }

        // Active light
        {
            const Light& light = lights[lightIndex];
            Matrix model = Translate(light.position.x, light.position.y, light.position.z);
            Matrix mvp = model * view * proj;
            glUseProgram(shaderColor);
            SendTransforms(mvp, model, cameraPosition, shaderColor);
            glUniform3fv(glGetUniformLocation(shaderColor, "u_color"), 1, (float*)&light.diffuse);
            glBindVertexArray(cube.vao);
            glDrawArrays(GL_TRIANGLES, 0, cube.vertexCount);

            if (lightVolume)
            {
                model = Scale(light.radius, light.radius, light.radius) * model;
                mvp = model * view * proj;
                SendTransforms(mvp, model, cameraPosition, shaderColor);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glBindVertexArray(sphere.vao);
                glDrawArrays(GL_TRIANGLES, 0, sphere.vertexCount);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }

        // Ground Plane
        {
            Matrix model = Scale(100.0f, 1.0f, 100.0f);
            Matrix mvp = model * view * proj;
            glUseProgram(shaderColor);
            SendTransforms(mvp, model, cameraPosition, shaderColor);
            glUniform3f(glGetUniformLocation(shaderColor, "u_color"), 0.375f, 0.375f, 0.375f);
            glBindVertexArray(plane.vao);
            glDrawArrays(GL_TRIANGLES, 0, plane.vertexCount);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SeparatorText("Attenuation Select");
        ImGui::RadioButton("x", (int*)&attenuation, LINEAR); ImGui::SameLine();
        ImGui::RadioButton("x2", (int*)&attenuation, EXP2); ImGui::SameLine();
        ImGui::RadioButton("x3", (int*)&attenuation, EXP3); ImGui::SameLine();
        ImGui::RadioButton("ease", (int*)&attenuation, EASE);
        ImGui::NewLine();

        ImGui::SeparatorText("Shader Select");
        ImGui::RadioButton("Gouraud", (int*)&shaderIndex, GOURAUD); ImGui::SameLine();
        ImGui::RadioButton("Phong", (int*)&shaderIndex, PHONG); ImGui::SameLine();
        ImGui::RadioButton("Phong Map", (int*)&shaderIndex, PHONG_MAPS);
        ImGui::NewLine();

        ImGui::SeparatorText("Light Select");
        ImGui::RadioButton("Light 1", (int*)&lightIndex, 0); ImGui::SameLine();
        ImGui::RadioButton("Light 2", (int*)&lightIndex, 1); ImGui::SameLine();
        ImGui::RadioButton("Light 3", (int*)&lightIndex, 2); ImGui::SameLine();
        ImGui::RadioButton("Light 4", (int*)&lightIndex, 3); ImGui::SameLine();
        ImGui::RadioButton("Light 5", (int*)&lightIndex, 4);
        ImGui::NewLine();

        static Vector3 color = Vector3One();
        static float ambient = 1.0f;
        static float diffuse = 1.0f;
        static float specular = 1.0f;
        bool change = false;
        Light& light = lights[lightIndex];
        ImGui::SeparatorText("Light Editor");
        change |= ImGui::ColorPicker3("Color", (float*)&color);
        change |= ImGui::SliderFloat("Ambient", &ambient, 0.0f, 1.0f);
        change |= ImGui::SliderFloat("Diffuse", &diffuse, 0.0f, 1.0f);
        change |= ImGui::SliderFloat("Specular", &specular, 0.0f, 1.0f);
        if (change)
        {
            light.ambient = color * ambient;
            light.diffuse = color * diffuse;
            light.specular = color * specular;
        }
        
        ImGui::NewLine();
        ImGui::SliderFloat3("Position", (float*)&light.position, -100.0f, 100.0f);
        ImGui::SliderFloat(" Radius", &light.radius, 1.0f, 50.0f);
        ImGui::Checkbox("Volume", &lightVolume);
        ImGui::NewLine();

        ImGui::SeparatorText("Camera");
        ImGui::SliderFloat3("Camera Position", (float*)&cameraPosition, -100.0f, 100.0f);
        ImGui::SliderAngle("Camera Pitch", (float*)&cameraPitch);
        ImGui::SliderAngle("Camera Yaw", (float*)&cameraYaw);

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