#pragma once
#include <Math.h>

struct PointLight {
    Vector3 position{};
    Vector3 ambient{};
    Vector3 diffuse{};
    Vector3 specular{};
    float radius = 1.0f;
};

struct DirectionLight
{
    Vector3 direction{};
    Vector3 ambient{};
    Vector3 diffuse{};
    Vector3 specular{};
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