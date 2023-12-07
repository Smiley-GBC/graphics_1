#pragma once
#include <Math.h>
#include <array>

struct PointLight {
    Vector3 position{};
    float radius = 1.0f;

    Vector3 ambient{};
    Vector3 diffuse{};
    Vector3 specular{};
};

struct DirectionLight
{
    Vector3 direction{};

    Vector3 ambient{};
    Vector3 diffuse{};
    Vector3 specular{};
};

struct SpotLight
{
    Vector3 position{};
    Vector3 direction{};
    float cutoff = 0.0f;

    Vector3 ambient{};
    Vector3 diffuse{};
    Vector3 specular{};
};

struct Lights
{
    SpotLight spotLight;
    DirectionLight directionLight;
    std::array<PointLight, 5> pointLights;
};

inline PointLight CreatePointLight(Vector3 position, Vector3 color, float ambient, float radius)
{
    PointLight light;
    light.ambient = color * ambient;
    light.diffuse = color;
    light.specular = { 1.0f, 1.0f, 1.0f };
    light.position = position;
    light.radius = radius;
    return light;
}

inline DirectionLight CreateDirectionLight(Vector3 direction, Vector3 color, float ambient)
{
    DirectionLight light;
    light.ambient = color * ambient;
    light.diffuse = color;
    light.specular = { 1.0f, 1.0f, 1.0f };
    light.direction = direction;
    return light;
}

inline SpotLight CreateSpotLight(Vector3 position, Vector3 direction, Vector3 color, float ambient, float cutoff/*degrees*/)
{
    SpotLight light;
    light.ambient = color * ambient;
    light.diffuse = color;
    light.specular = { 1.0f, 1.0f, 1.0f };
    light.position = position;
    light.direction = direction;
    light.cutoff = cosf(cutoff * 0.5f * DEG2RAD);
    return light;
}
