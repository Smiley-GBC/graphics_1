#include "Physics.h"

struct PhysicsSystem
{
	std::vector<Entity> entities;
	size_t id = 0;
} gPhysics;

void Simulate(Entity& entity, Vector3 acc, float dt)
{
	Body& body = entity.body;
	body.vel = body.vel + acc * body.mass * body.gravityScale * dt;
	entity.pos = entity.pos + body.vel * dt;
}

void SimulateAll(Vector3 acc, float dt)
{
	for (Entity& entity : gPhysics.entities)
		Simulate(entity, acc, dt);
	

}

Entity& Add(Entity entity)
{
	entity.id = ++gPhysics.id;
	gPhysics.entities.push_back(entity);
	return gPhysics.entities.back();
}

void Remove(size_t id)
{
	for (size_t i = 0; i < gPhysics.entities.size(); i++)
	{
		// TODO -- use remove_if for batch-erase
		if (id == gPhysics.entities[i].id)
		{
			gPhysics.entities.erase(gPhysics.entities.begin() + i);
			break;
		}
	}
}

std::vector<Entity>& All()
{
	return gPhysics.entities;
}
