#include "Physics.h"

void Simulate(Entity& entity, Vector3 acc, float dt)
{
	Body& body = entity.body;
	body.vel = body.vel + acc * body.gravityScale * body.mass * dt;
	entity.pos = entity.pos + body.vel * dt;
}
