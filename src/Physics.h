#pragma once
#include <Math.h>
#include "Color.h"

struct Body
{
	Vector3 vel{};
	float mass = 1.0f;
	float gravityScale = 1.0f;
};

struct Entity
{
	size_t id = 0;
	Vector3 pos{};
	Body body;
	Color color;
};

void Simulate(Entity& entity, Vector3 acc, float dt);