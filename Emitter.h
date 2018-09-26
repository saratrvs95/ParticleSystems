#pragma once
#include "Particle.h"

namespace ParticleSystem
{
	class IEmitter
	{
	public:
		IEmitter() = default;
		virtual ~IEmitter() = default;
		virtual void EmitParticle(Particle& particle) = 0;
	};
}