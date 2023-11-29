#pragma once
#include <Math.h>

constexpr float SPECULAR_POWER = 256.0f;

struct Material {
    Vector3 ambient{};
    Vector3 diffuse{};
    Vector3 specular{};
    float shininess = 1.0f;
    // % influence of specular power
};

extern Material emerald;
extern Material jade;
extern Material obsidian;
extern Material pearl;
extern Material ruby;
extern Material turquoise;
extern Material brass;
extern Material bronze;
extern Material chrome;
extern Material copper;
extern Material gold;
extern Material silver;

void CreateMaterials();