#pragma once
#include <Math.h>
#include "Color.h"
#include <vector>

// "Physics Component"
struct Body
{
	Vector3 vel{};
	float mass = 1.0f;
	float gravityScale = 1.0f;
};

// "Game Object"
struct Entity
{
	size_t id = 0;
	Vector3 pos{};
	Body body;
	Color color;
};

void Simulate(Entity& entity, Vector3 acc, float dt);
void SimulateAll(Vector3 acc, float dt);

Entity& Add(Entity entity);
void Remove(size_t id);

// Hack that allows us to conveniently render our entities
std::vector<Entity>& All();