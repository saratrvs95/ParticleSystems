#include "pch.h"
#include "SphereEmitter.h"
#include "Random.h"

namespace ParticleSystem
{
	SphereEmitter::SphereEmitter() : 
		mOrigin(0.0f, 0.0f, 0.0f), mRadius(1.0f)
	{
	}

	SphereEmitter::SphereEmitter(glm::vec3 position, float radius) : 
		mOrigin(position), mRadius(radius)
	{
	}

	SphereEmitter::SphereEmitter(glm::vec3 position, float radius, glm::vec3 initialVelocity) :
		mOrigin(position), mRadius(radius), mInitialParticleVelocity(initialVelocity)
	{
		mUseInitialVelocity = true;
	}

	void SphereEmitter::EmitParticle(Particle & particle)
	{
		float inclination = sinf(GetRandomAltitude());
		float yaw = GetRandomAzimuth();

		float X = inclination * cosf(yaw);
		float Y = cosf(inclination);
		float Z = inclination * sinf(yaw);
		glm::vec3 directionVector(X, Y, Z);


		glm::vec3 spawnpoint;
		switch (mEmissionType)
		{
		case ParticleSystem::EmissionType::Volume:
			spawnpoint = GetInternalPoint();
			break;

		case ParticleSystem::EmissionType::Surface:
			spawnpoint = GetSurfacePoint();
			break;

		default:
			break;
		}
		particle.Position = directionVector + spawnpoint;
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

	void SphereEmitter::SetParticleSpeedRange(float minSpeed, float maxSpeed)
	{
		mMinParticleSpeed = minSpeed;
		mMaxParticleSpeed = maxSpeed;
	}

	float SphereEmitter::GetRandomSpeed()
	{
		return Random::RandRange(mMinParticleSpeed, mMaxParticleSpeed);
	}

	void SphereEmitter::SetParticleLifetimeRange(float minLifetime, float maxLifetime)
	{
		mMinParticleLifetime = minLifetime;
		mMaxParticleLifetime = maxLifetime;
	}

	float SphereEmitter::GetRandomLifetime()
	{
		return Random::RandRange(mMinParticleLifetime, mMaxParticleLifetime);
	}

	void SphereEmitter::SetAzimuthRange(float minAzimuth, float maxAzimuth)
	{
		mMinAzimuth = minAzimuth;
		mMaxAzimuth = maxAzimuth;
	}

	void SphereEmitter::SetAltitudeRange(float minAltitude, float maxAltitude)
	{
		mMinAltitude = minAltitude;
		mMaxAltitude = maxAltitude;
	}

	void SphereEmitter::SetRadius(float radius)
	{
		mRadius = radius;
	}

	void SphereEmitter::SetEmissionType(EmissionType type)
	{
		mEmissionType = type;
	}

	void SphereEmitter::SetPosition(glm::vec3 position)
	{
		mOrigin = position;
	}

	float SphereEmitter::GetRandomAzimuth()
	{
		return glm::radians(Random::RandRange(mMinAzimuth, mMaxAzimuth));
	}

	float SphereEmitter::GetRandomAltitude()
	{
		//Same as inclination
		return glm::radians(Random::RandRange(mMinAltitude, mMaxAltitude));
	}

	glm::vec3 SphereEmitter::GetInternalPoint()
	{
		float length = Random::RandRange(0.0f, mRadius);
		float azimuth = GetRandomAzimuth();
		float altitude = GetRandomAltitude();
		float pi = 3.142f;

		float x = (cosf(azimuth) * sinf(altitude) * length) + mOrigin.x;
		float y = (sinf(azimuth) * sinf(altitude) * length) + mOrigin.y;
		float z = (cosf(altitude) * length) + mOrigin.z;

		/*if (altitude > (3 / 2) * pi || altitude <= pi / 2)
		{
			y = ((1 - cosf(altitude)) * length) + mOrigin.y;
		}
		else if (altitude > pi / 2 || altitude <= (3 / 2) * pi)
		{
			y = ((1 - cosf(altitude)) * length) - mOrigin.y;
		}*/

		return glm::vec3(x, y, z);
	}

	glm::vec3 SphereEmitter::GetSurfacePoint()
	{
		float azimuth = GetRandomAzimuth();
		float altitude = GetRandomAltitude();
		float pi = 3.142f;

		float x = (cosf(azimuth) * sinf(altitude) * mRadius) + mOrigin.x;
		float y = (sinf(azimuth) * sinf(altitude) * mRadius) + mOrigin.y;
		float z = (cosf(altitude) * mRadius) + mOrigin.z;

		/*if (altitude > (3 / 2) * pi || altitude <= pi / 2)
		{
			y = ((1 - cosf(altitude)) * mRadius) + mOrigin.y;
		}
		else if (altitude > pi / 2 || altitude <= (3 / 2) * pi)
		{
			y = ((-1 + cosf(altitude)) * mRadius) - mOrigin.y;
		}*/
		
		return glm::vec3(x, y, z);
	}

}