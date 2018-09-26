#pragma once
#include "Emitter.h"

namespace ParticleSystem
{
	class PointEmitter : public IEmitter
	{
	public:
		PointEmitter();
		PointEmitter(glm::vec3 origin);
		PointEmitter(glm::vec3 origin, glm::vec3 initialVelocity);
		~PointEmitter() = default;

		virtual void EmitParticle(Particle& particle) override;

		void SetParticleSpeedRange(float minSpeed, float maxSpeed);
		float GetRandomSpeed();

		void SetParticleLifetimeRange(float minLifetime, float maxLifetime);
		float GetRandomLifetime();

		void SetPosition(glm::vec3 position);

	private:
		float GetRandomInclination();
		float GetRandomYaw();

		float mMaxSize = 10.0f;
		float mMinSize = 20.0f;

		float mMinParticleSpeed = 10.0f;
		float mMaxParticleSpeed = 20.0f;

		float mMinParticleLifetime = 2.0f;
		float mMaxParticleLifetime = 5.0f;

		float mMinInclination = 0.0f;
		float mMaxInclination = 180.0f;

		float mMinYaw = 0.0f;
		float mMaxYaw = 360.0f;

		glm::vec3 mSpawnPoint;
		glm::vec3 mInitialParticleVelocity;
		bool mUseInitialVelocity = false;
	};
}
