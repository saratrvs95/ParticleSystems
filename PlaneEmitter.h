#pragma once
#include "Emitter.h"
#include "EmissionType.h"

namespace ParticleSystem
{
	class BoxEmitter : public IEmitter
	{
	public:
		BoxEmitter();
		BoxEmitter(glm::vec3 position, glm::vec3 size);
		BoxEmitter(glm::vec3 position, glm::vec3 size, glm::vec3 initialVelocity);
		~BoxEmitter() = default;

		virtual void EmitParticle(Particle& particle) override;

		void SetParticleSpeedRange(float minSpeed, float maxSpeed);
		float GetRandomSpeed();

		void SetParticleLifetimeRange(float minLifetime, float maxLifetime);
		float GetRandomLifetime();

		void SetSize(glm::vec3 size);
		void SetEmissionType(EmissionType type);
		void SetPosition(glm::vec3 position);

	private:
		float GetRandomInclination();
		float GetRandomYaw();
		glm::vec3 GetRandomSpawnpoint();
		glm::vec3 GetInternalPoint();
		glm::vec3 GetSurfacePoint(float inclination, float yaw);

		float mMinParticleSpeed = 10.0f;
		float mMaxParticleSpeed = 20.0f;

		float mMinParticleLifetime = 2.0f;
		float mMaxParticleLifetime = 5.0f;

		float mMinInclination = 0.0f;
		float mMaxInclination = 180.0f;

		float mMinYaw = 0.0f;
		float mMaxYaw = 360.0f;

		glm::vec3 mSize;

		glm::vec3 mOrigin;
		EmissionType mEmissionType;
		glm::vec3 mInitialParticleVelocity;
		bool mUseInitialVelocity = false;
		bool bTest = false;
	};
}
