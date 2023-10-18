#include "Physics.h"

struct PhysicsSystem
{
	size_t id = 0;
	Entities entities;
	HitPairs collisions;
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

	gPhysics.collisions.clear();
	for (size_t i = 0; i < gPhysics.entities.size(); i++)
	{
		Entity& a = gPhysics.entities[i];
		for (size_t j = 0; j < gPhysics.entities.size(); j++)
		{
			if (i == j) continue;	// Don't check self for collision

			Entity& b = gPhysics.entities[j];
			bool collision = false;

			if (a.shapeType == SPHERE && b.shapeType == SPHERE)
			{
				collision = CheckCollisionSpheres(a.pos, a.shape.sphere.radius, b.pos, b.shape.sphere.radius);
			}

			if (a.shapeType == SPHERE && b.shapeType == PLANE)
			{
				collision = CheckCollisionSpherePlane(a.pos, a.shape.sphere.radius, b.pos, b.shape.plane.normal);
			}

			if (a.shapeType == PLANE && b.shapeType == SPHERE)
			{
				collision = CheckCollisionSpherePlane(b.pos, b.shape.sphere.radius, a.pos, a.shape.plane.normal);
			}

			if (collision)
			{
				HitPair pair;
				pair.a = &a;
				pair.b = &b;
				gPhysics.collisions.push_back(pair);
			}
		}
	}
}

HitPairs& Collisions()
{
	return gPhysics.collisions;
}

size_t Add(Shape shape, ShapeType shapeType, Vector3 pos, Vector3 dir)
{
	Entity entity;
	entity.id = ++gPhysics.id;
	entity.pos = pos;
	entity.dir = dir;
	entity.shape = shape;
	entity.shapeType = shapeType;

	gPhysics.entities.push_back(entity);
	return gPhysics.entities.back().id;
}

Entity& Get(size_t id)
{
	for (Entity& entity : gPhysics.entities)
	{
		if (entity.id == id)
			return entity;
	}
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

bool CheckCollisionSpheres(Vector3 pos1, float radius1, Vector3 pos2, float radius2)
{
	return DistanceSqr(pos1, pos2) <= (radius1 + radius2) * (radius1 + radius2);
}

bool CheckCollisionSpherePlane(Vector3 spherePos, float sphereRadius, Vector3 planePos, Vector3 planeNormal)
{
	// TODO -- add this for homework ;)
	return false;
}
