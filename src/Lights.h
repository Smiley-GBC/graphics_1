#pragma once
#include <Math.h>

struct Light {
    Vector3 position{};
    Vector3 ambient{};
    Vector3 diffuse{};
    Vector3 specular{};
    float radius = 1.0f;
};

inline Light CreateLight(Vector3 position, Vector3 color, float ambient, float radius)
{
    Light light;
    light.ambient = color * ambient;
    light.diffuse = color;
    light.specular = { 1.0f, 1.0f, 1.0f };
}