#pragma once
#include <Math.h>
#include "Color.h"

// Physics information
struct Body
{
	Vector3 vel{};
	float mass = 1.0f;
	float gravityScale = 1.0f;
};

// Game object that owns physics information, and many other things!
struct Entity
{
	Vector3 pos;
	Body body;
};

void Simulate(Entity& entity, Vector3 acc, float dt);