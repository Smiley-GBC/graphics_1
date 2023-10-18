#include "Physics.h"

struct PhysicsSystem
{
	size_t id = 0;
	Entities entities;
} gPhysics;

void Simulate(Entity& entity, Vector3 acc, float dt)
{
	Body& body = entity.body;
	body.vel = body.vel + acc * body.gravityScale * body.mass * dt;
	entity.pos = entity.pos + body.vel * dt;
}

void SimulateAll(Vector3 acc, float dt)
{
	for (Entity& entity : gPhysics.entities)
		Simulate(entity, acc, dt);
}

size_t Add(Vector3 pos)
{
	Entity entity;
	entity.id = ++gPhysics.id;
	entity.pos = pos;

	gPhysics.entities.push_back(entity);
	return gPhysics.entities.back().id;
}

void Remove(size_t id)
{
	for (size_t i = 0; i < gPhysics.entities.size(); i++)
	{
		if (id == gPhysics.entities[i].id)
		{
			gPhysics.entities.erase(gPhysics.entities.begin() + 1);
			break;
		}
	}
}

Entities& All()
{
	return gPhysics.entities;
}
