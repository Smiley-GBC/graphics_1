#pragma once
#include <Math.h>
#include "Color.h"
#include <vector>

struct Sphere
{
	float radius;
};

struct Plane
{
	Vector3 normal;
};

union Shape
{
	Sphere sphere;
	Plane plane;
};

enum ShapeType
{
	SPHERE,
	PLANE
};

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
	Shape shape;
	ShapeType shapeType;
	Color color;
};

struct HitPair
{
	Entity* a = nullptr;
	Entity* b = nullptr;
	// TODO -- add collision normal & collision depth
};

inline void Simulate(Entity& entity, Vector3 acc, float dt)
{
	Body& body = entity.body;
	body.vel = body.vel + acc * body.mass * body.gravityScale * dt;
	entity.pos = entity.pos + body.vel * dt;
}

void SimulateAll(Vector3 acc, float dt);

size_t Add(Entity entity);
void Remove(size_t id);

// Note -- this Entity& will be invalidated if entities are added/removed
Entity& Get(size_t id);

// Hack that allows us to conveniently render our entities
std::vector<Entity>& All();
std::vector<HitPair>& Hits();

inline bool CheckCollisionSpheres(Vector3 pos1, float radius1, Vector3 pos2, float radius2)
{
	// Optimization: compare squared distance to squared radii sum to remove expensive sqrt
	return DistanceSqr(pos1, pos2) <= (radius1 + radius2) * (radius1 + radius2);
}

inline bool CheckCollisionSpherePlane(Vector3 posSphere, float radius, Vector3 posPlane, Vector3 normal)
{
	return Dot(posSphere - posPlane, normal) <= radius;
}
