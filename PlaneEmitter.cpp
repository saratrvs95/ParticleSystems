#include "pch.h"
#include "PlaneEmitter.h"
#include "Random.h"

namespace ParticleSystem
{
	BoxEmitter::BoxEmitter() :
		mOrigin(glm::vec3(0))
	{
	}

	BoxEmitter::BoxEmitter(glm::vec3 position, glm::vec3 size) :
		mOrigin(position), mSize(size)
	{
	}

	BoxEmitter::BoxEmitter(glm::vec3 position, glm::vec3 size, glm::vec3 initialVelocity) :
		mOrigin(position), mSize(size), mInitialParticleVelocity(initialVelocity)
	{
		mUseInitialVelocity = true;
	}

	void BoxEmitter::EmitParticle(Particle & particle)
	{
		float inclination = sinf(GetRandomInclination());
		float yaw = GetRandomYaw();

		float X = inclination * cosf(yaw);
		float Y = inclination * sinf(yaw);
		float Z = cosf(inclination);
		glm::vec3 directionVector(X, Y, Z);

		//glm::vec3 spawnPoint = GetRandomSpawnpoint();
		glm::vec3 spawnPoint;
		switch (mEmissionType)
		{
		case ParticleSystem::EmissionType::Volume:
			spawnPoint = GetInternalPoint();
			break;

		case ParticleSystem::EmissionType::Surface:
			spawnPoint = GetSurfacePoint(inclination, yaw);;
			break;

		default:
			break;
		}
		particle.Position = directionVector + spawnPoint;
		if (mUseInitialVelocity)
		{
			particle.Velocity = directionVector * mInitialParticleVelocity;
			mUseInitialVelocity = false;
		}
		else
		{
			particle.Velocity = directionVector * GetRandomSpeed();
		}
		particle.LifeTime = GetRandomLifetime();
		particle.Age = 0.0f;
	}

	void BoxEmitter::SetParticleSpeedRange(float minSpeed, float maxSpeed)
	{
		mMinParticleSpeed = minSpeed;
		mMaxParticleSpeed = maxSpeed;
	}

	float BoxEmitter::GetRandomSpeed()
	{
		return Random::RandRange(mMinParticleSpeed, mMaxParticleSpeed);
	}

	void BoxEmitter::SetParticleLifetimeRange(float minLifetime, float maxLifetime)
	{
		mMinParticleLifetime = minLifetime;
		mMaxParticleLifetime = maxLifetime;
	}

	float BoxEmitter::GetRandomLifetime()
	{
		return Random::RandRange(mMinParticleLifetime, mMaxParticleLifetime);
	}

	void BoxEmitter::SetSize(glm::vec3 size)
	{
		mSize = size;
	}

	void BoxEmitter::SetEmissionType(EmissionType type)
	{
		mEmissionType = type;
	}

	void BoxEmitter::SetPosition(glm::vec3 position)
	{
		mOrigin = position;
	}

	float BoxEmitter::GetRandomInclination()
	{
		return glm::radians(Random::RandRange(mMinInclination, mMaxInclination));
	}

	float BoxEmitter::GetRandomYaw()
	{
		return glm::radians(Random::RandRange(mMinYaw, mMaxYaw));
	}

	glm::vec3 BoxEmitter::GetRandomSpawnpoint()
	{
		glm::vec3 halfSize = glm::vec3(mSize.x / 2, mSize.y / 2, mSize.z / 2);
		float x = Random::RandRange((mOrigin.x - halfSize.x), (mOrigin.x + halfSize.x));
		float y = Random::RandRange((mOrigin.y - halfSize.y), (mOrigin.y + halfSize.y));
		float z = Random::RandRange((mOrigin.z - halfSize.z), (mOrigin.z + halfSize.z));
		return glm::vec3(x, y, z);
	}

	glm::vec3 BoxEmitter::GetInternalPoint()
	{
		glm::vec3 halfSize = glm::vec3(mSize.x / 2, mSize.y / 2, mSize.z / 2);
		float x = Random::RandRange((mOrigin.x - halfSize.x), (mOrigin.x + halfSize.x));
		float y = Random::RandRange((mOrigin.y - halfSize.y), (mOrigin.y + halfSize.y));
		float z = Random::RandRange((mOrigin.z - halfSize.z), (mOrigin.z + halfSize.z));
		return glm::vec3(x, y, z);
	}

	glm::vec3 BoxEmitter::GetSurfacePoint(float inclination, float yaw)
	{
		glm::vec3 halfSize = glm::vec3(mSize.x / 2, mSize.y / 2, mSize.z / 2);
		float x;
		float y;
		float z;

		if (inclination >= glm::radians(0.0f) && inclination < glm::radians(45.0f))
		{
			if (yaw >= glm::radians(0.0f) && yaw < glm::radians(45.0f))
			{
				x = Random::RandRange((mOrigin.x - halfSize.x), (mOrigin.x + halfSize.x));
				y = Random::RandRange((mOrigin.y - halfSize.y), (mOrigin.y + halfSize.y));
				z = mOrigin.z + halfSize.z;
			}
			else if (yaw >= glm::radians(45.0f) && yaw < glm::radians(135.0f))
			{
				x = mOrigin.x + halfSize.x;
				y = Random::RandRange((mOrigin.y - halfSize.y), (mOrigin.y + halfSize.y));
				z = Random::RandRange((mOrigin.z - halfSize.z), (mOrigin.z + halfSize.z));
			}
			else if (yaw >= glm::radians(135.0f) && yaw < glm::radians(225.0f))
			{
				x = Random::RandRange((mOrigin.x - halfSize.x), (mOrigin.x + halfSize.x));
				y = Random::RandRange((mOrigin.y - halfSize.y), (mOrigin.y + halfSize.y));
				z = mOrigin.z - halfSize.z;
			}
			else if (yaw >= glm::radians(225.0f) && yaw < glm::radians(315.0f))
			{
				x = mOrigin.x - halfSize.x;
				y = Random::RandRange((mOrigin.y - halfSize.y), (mOrigin.y + halfSize.y));
				z = Random::RandRange((mOrigin.z - halfSize.z), (mOrigin.z + halfSize.z));
			}
			else
			{
				x = Random::RandRange((mOrigin.x - halfSize.x), (mOrigin.x + halfSize.x));
				y = Random::RandRange((mOrigin.y - halfSize.y), (mOrigin.y + halfSize.y));
				z = mOrigin.z + halfSize.z;
			}
		}
		else if (inclination >= glm::radians(45.0f) && inclination < glm::radians(90.0f))
		{
			x = Random::RandRange((mOrigin.x - halfSize.x), (mOrigin.x + halfSize.x));
			
			z = Random::RandRange((mOrigin.z - halfSize.z), (mOrigin.z + halfSize.z));
			if (bTest)
			{
				y = mOrigin.y + halfSize.y;
				bTest = false;
			}
			else
			{
				y = mOrigin.y - halfSize.y;
				bTest = true;
			}
			
		}
		
		/*float x = Random::RandRange((mOrigin.x - halfSize.x), (mOrigin.x + halfSize.x));
		float y = Random::RandRange((mOrigin.y - halfSize.y), (mOrigin.y + halfSize.y));
		float z = Random::RandRange((mOrigin.z - halfSize.z), (mOrigin.z + halfSize.z));*/

		/*if (x > mOrigin.x)
		{
			x = mOrigin.x + halfSize.x;
			return glm::vec3(x, y, z);
		}
		else if(y > mOrigin.y)
		{
			y = mOrigin.y + halfSize.y;
			return glm::vec3(x, y, z);
		}
		else if (x > mOrigin.x && y > mOrigin.y && z > mOrigin.z)
		{
			z = mOrigin.z + halfSize.z;
			return glm::vec3(x, y, z);
		}*/

		/*if (y > mOrigin.y)
		{
			y = mOrigin.y + halfSize.y;
			return glm::vec3(x, y, z);
		}
		else
		{
			y = mOrigin.y - halfSize.y;
			return glm::vec3(x, y, z);
		}

		if (z > mOrigin.z)
		{
			z = mOrigin.z + halfSize.z;
			return glm::vec3(x, y, z);
		}
		else
		{
			z = mOrigin.z - halfSize.z;
			return glm::vec3(x, y, z);
		}*/
		return glm::vec3(x, y, z);
	}
}