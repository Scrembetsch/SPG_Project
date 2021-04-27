#pragma once

#include <glm/glm.hpp>

struct Particle
{
	enum Type
	{
		PARTICLE = 0,
		GENERATOR = 1
	};

	glm::vec3 Position;
	glm::vec3 Velocity;
	glm::vec3 Color;
	float LifeTime;
	float Size;
	Type Type;
};