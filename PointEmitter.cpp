#include "pch.h"
#include "PointEmitter.h"
#include "Random.h"

namespace ParticleSystem
{
	PointEmitter::PointEmitter() : 
		mSpawnPoint(glm::vec3(0)), mInitialParticleVelocity(glm::vec3(0))
	{
	}

	PointEmitter::PointEmitter(glm::vec3 origin) : 
		mSpawnPoint(origin)
	{
	}

	PointEmitter::PointEmitter(glm::vec3 origin, glm::vec3 initialVelocity) :
		mSpawnPoint(origin), mInitialParticleVelocity(initialVelocity)
	{
		mUseInitialVelocity = true;
	}

	void PointEmitter::EmitParticle(Particle & particle)
	{
		float inclination = sinf(GetRandomInclination());
		float yaw = GetRandomYaw();

		float X = inclination * cosf(yaw);
		float Y = cosf(inclination);
		float Z = inclination * sinf(yaw);
		glm::vec3 directionVector(X, Y, Z);

		particle.Position = directionVector + mSpawnPoint;
		if (mUseInitialVelocity)
		{
			particle.Velocity = mInitialParticleVelocity;
			mUseInitialVelocity = false;
		}
		else
		{
			particle.Velocity = directionVector * GetRandomSpeed();
		}
		particle.LifeTime = GetRandomLifetime();
		particle.Age = 0.0f;
	}

	void PointEmitter::SetParticleSpeedRange(float minSpeed, float maxSpeed)
	{
		mMinParticleSpeed = minSpeed;
		mMaxParticleSpeed = maxSpeed;
	}

	float PointEmitter::GetRandomSpeed()
	{
		return Random::RandRange(mMinParticleSpeed, mMaxParticleSpeed);
	}

	void PointEmitter::SetParticleLifetimeRange(float minLifetime, float maxLifetime)
	{
		mMinParticleLifetime = minLifetime;
		mMaxParticleLifetime = maxLifetime;
	}

	float PointEmitter::GetRandomLifetime()
	{
		return Random::RandRange(mMinParticleLifetime, mMaxParticleLifetime);
	}

	void PointEmitter::SetPosition(glm::vec3 position)
	{
		mSpawnPoint = position;
	}

	float PointEmitter::GetRandomInclination()
	{
		return glm::radians(Random::RandRange(mMinInclination, mMaxInclination));
	}

	float PointEmitter::GetRandomYaw()
	{
		return glm::radians(Random::RandRange(mMinYaw, mMaxYaw));
	}
}