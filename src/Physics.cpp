#include "Physics.h"

struct PhysicsSystem
{
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

Entity* Add(Vector3 pos)
{
	Entity entity;
	entity.pos = pos;
	gPhysics.entities.push_back(entity);
	return &gPhysics.entities.back();
}

void Remove(Entity* entity)
{
	for (size_t i = 0; i < gPhysics.entities.size(); i++)
	{
		if (entity == &gPhysics.entities[i])
		{
			gPhysics.entities.erase(gPhysics.entities.begin() + 1);
			break;
		}
	}
}
