#pragma once
#include "Emitter.h"
#include "EmissionType.h"

namespace ParticleSystem
{
	class SphereEmitter : public IEmitter
	{
	public:
		SphereEmitter();
		SphereEmitter(glm::vec3 position, float radius);
		SphereEmitter(glm::vec3 position, float radius, glm::vec3 initialVelocity);
		~SphereEmitter() = default;

		virtual void EmitParticle(Particle& particle) override;

		void SetParticleSpeedRange(float minSpeed, float maxSpeed);
		float GetRandomSpeed();

		void SetParticleLifetimeRange(float minLifetime, float maxLifetime);
		float GetRandomLifetime();

		void SetAzimuthRange(float minAzimuth, float maxAzimuth);
		void SetAltitudeRange(float minAltitude, float maxAltitude);

		void SetRadius(float radius);
		void SetEmissionType(EmissionType type);
		void SetPosition(glm::vec3 position);

	private:
		float GetRandomAzimuth();
		float GetRandomAltitude();
		glm::vec3 GetInternalPoint();
		glm::vec3 GetSurfacePoint();

		float mMinParticleSpeed = 10.0f;
		float mMaxParticleSpeed = 20.0f;

		float mMinParticleLifetime = 2.0f;
		float mMaxParticleLifetime = 5.0f;

		//setters for azimuth and altitude
		float mMinAzimuth = 0.0f;
		float mMaxAzimuth = 360.0f;

		float mMinAltitude = 0.0f;
		float mMaxAltitude = 180.0f;

		float mRadius;

		glm::vec3 mOrigin;
		EmissionType mEmissionType;
		glm::vec3 mInitialParticleVelocity;
		bool mUseInitialVelocity = false;
	};
}
