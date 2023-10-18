#pragma once
#include <Math.h>
#include "Color.h"
#include <vector>

enum ShapeType
{
	NONE,
	SPHERE,
	PLANE
};

struct SphereShape
{
	float radius;
};

struct PlaneShape
{
	Vector3 normal;
};

union Shape
{
	SphereShape sphere;
	PlaneShape plane;
};

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
	Shape shape;
	ShapeType shapeType;
	Color color;
};
using Entities = std::vector<Entity>;
using Ids = std::vector<size_t>;

struct HitPair
{
	Entity* a;
	Entity* b;
};
using HitPairs = std::vector<HitPair>;

void Simulate(Entity& entity, Vector3 acc, float dt);
void SimulateAll(Vector3 acc, float dt);
HitPairs& Collisions();

size_t Add(Vector3 pos = {});
void Remove(size_t id);
Entities& All();

bool CheckCollisionSpheres(Vector3 pos1, float radius1, Vector3 pos2, float radius2);
bool CheckCollisionSpherePlane(Vector3 spherePos, float sphereRadius, Vector3 planePos, Vector3 planeNormal);