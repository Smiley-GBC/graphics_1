#pragma once
#include <Math.h>
#include "Color.h"
#include <vector>

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
using Entities = std::vector<Entity>;

void Simulate(Entity& entity, Vector3 acc, float dt);
void SimulateAll(Vector3 acc, float dt);

Entity* Add(Vector3 pos = {});
void Remove(Entity* entity);