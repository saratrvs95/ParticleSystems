#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace ParticleSystem
{
	struct Particle
	{
		Particle()
			: Position(0)
			, Velocity(0)
			, Color(0)
		{}

		glm::vec3 Position;		// Center point of particle
		glm::vec3 Velocity;		// Current particle velocity
		glm::vec4 Color;		// Particle color

		float Rotate = 0.0f;	// Rotate the particle
		float Size;				// Size of the particle
		float Age = 0.0f;
		float LifeTime = 0.0f;
	};

	using ParticleBuffer = std::vector<Particle>;
}