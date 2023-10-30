#include "Physics.h"
#include <cassert>

struct PhysicsSystem
{
	std::vector<Entity> entities;
	std::vector<HitPair> hits;
	size_t id = 0;
} gPhysics;

void SimulateAll(Vector3 acc, float dt)
{
	for (Entity& entity : gPhysics.entities)
		Simulate(entity, acc, dt);
	
	// Detect overlapping pairs, then resolve them
	gPhysics.hits.clear();
	for (size_t i = 0; i < gPhysics.entities.size(); i++)
	{
		for (size_t j = 0; j < gPhysics.entities.size(); j++)
		{
			// Don't check for collision against self
			if (i == j)
				continue;

			bool collision = false;
			Entity& a = gPhysics.entities[i];
			Entity& b = gPhysics.entities[j];

			if (a.shapeType == SPHERE && b.shapeType == SPHERE)
			{
				collision =
					CheckCollisionSpheres(a.pos, a.shape.sphere.radius, b.pos, b.shape.sphere.radius);
			}
			else if (a.shapeType == SPHERE && b.shapeType == PLANE)
			{
				collision =
					CheckCollisionSpherePlane(a.pos, a.shape.sphere.radius, b.pos, b.shape.plane.normal);
			}
			else if (a.shapeType == PLANE && b.shapeType == SPHERE)
			{
				collision =
					CheckCollisionSpherePlane(b.pos, b.shape.sphere.radius, a.pos, a.shape.plane.normal);
			}
			else
			{
				assert(false, "Invalid shape type!");
			}

			if (collision)
			{
				gPhysics.hits.push_back({ &a, &b });
			}
		}
	}
}

size_t Add(Entity entity)
{
	entity.id = ++gPhysics.id;
	gPhysics.entities.push_back(entity);
	return gPhysics.entities.back().id;
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

Entity& Get(size_t id)
{
	for (Entity& entity : All())
	{
		if (id == entity.id)
			return entity;
	}
}

std::vector<Entity>& All()
{
	return gPhysics.entities;
}

std::vector<HitPair>& Hits()
{
	return gPhysics.hits;
}
