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
	size_t id = 0;
	Vector3 pos;
	Body body;
};
using Entities = std::vector<Entity>;
using Ids = std::vector<size_t>;

void Simulate(Entity& entity, Vector3 acc, float dt);
void SimulateAll(Vector3 acc, float dt);

size_t Add(Vector3 pos = {});
void Remove(size_t id);
Entities& All();