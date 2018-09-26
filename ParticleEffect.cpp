#include "pch.h"
#include <learnopengl/camera.h>
#include "Random.h"
#include "ParticleEffect.h"


namespace ParticleSystem
{
	ParticleEffect::ParticleEffect(uint32_t particleCount) : 
		mCamera(nullptr), mEmitter(nullptr), mColorInterpolator(glm::vec4(1.0f)), mWorldTransformMatrix(1), mGravity(0.0f, -9.81f, 0.f)
	{
		Resize(particleCount);
	}

	ParticleEffect::~ParticleEffect()
	{
		if (mTextureID != 0)
		{
			glDeleteTextures(1, &mTextureID);
			mTextureID = 0;
		}
	}

	void ParticleEffect::SetWindow(GLFWwindow * window)
	{
		mWindow = window;
	}

	void ParticleEffect::SetCamera(Camera* camera)
	{
		mCamera = camera;
	}

	void ParticleEffect::SetEmitter(IEmitter* emitter)
	{
		mEmitter = emitter;
	}

	void ParticleEffect::SetColorInterpolator(const ColorInterpolator& color)
	{
		mColorInterpolator = color;
	}

	void ParticleEffect::SetVelocityInterpolator(const VelocityInterpolator & velocity)
	{
		mVelocityInterpolator = velocity;
	}

	void ParticleEffect::SetForceInterpolator(const ForceInterpolator & force)
	{
		mForceInterpolator = force;
	}

	void ParticleEffect::SetGravityScale(float scale)
	{
		mGravityScale = scale;
	}

	void ParticleEffect::SetForce(glm::vec3 force)
	{
		mForce = force;
	}

	void ParticleEffect::SetBlendingMode(BlendingMode blendMode)
	{
		mBlendingMode = blendMode;
	}

	void ParticleEffect::InitializeEffect(GLFWwindow * window, Camera * camera, IEmitter * emitter)
	{
		SetWindow(window);
		SetCamera(camera);
		SetEmitter(emitter);
	}

	void ParticleEffect::InitializeEffect(GLFWwindow * window, Camera * camera, IEmitter * emitter, glm::vec3 initialVelocity)
	{
		SetWindow(window);
		SetCamera(camera);
		SetEmitter(emitter);
		
	}

	void ParticleEffect::SetEffectParameters(ColorInterpolator * colorInterpolator, VelocityInterpolator * velocityInterpolator, ForceInterpolator * forceInterpolator, BlendingMode blend)
	{
		if (colorInterpolator != nullptr)
		{
			SetColorInterpolator(*colorInterpolator);
		}
		if (velocityInterpolator != nullptr)
		{
			SetVelocityInterpolator(*velocityInterpolator);
		}
		if (forceInterpolator != nullptr)
		{
			SetForceInterpolator(*forceInterpolator);
		}
		SetBlendingMode(blend);
	}

	void ParticleEffect::RandomizeParticles()
	{
		for (auto& particle : mParticles)
		{
			RandomizeParticle(particle);
		}
	}

	void ParticleEffect::EmitParticles()
	{
		for (auto& particle : mParticles)
		{
			EmitParticle(particle);
		}
	}

	void ParticleEffect::Update(float deltaTime)
	{
		for (auto& particle : mParticles)
		{
			particle.Age += deltaTime;
			if (particle.Age > particle.LifeTime)
			{
				if (mEmitter != nullptr)
				{
					EmitParticle(particle);
				}
				else
				{
					RandomizeParticle(particle);
				}
			}
			float lifeRatio = glm::saturate<float, glm::precision::defaultp>(particle.Age / particle.LifeTime);

			mForce = mForceInterpolator.GetValue(lifeRatio);
			float velocityMultiplier = mVelocityInterpolator.GetValue(lifeRatio);
			particle.Velocity = (particle.Velocity * velocityMultiplier) + (((mGravity * mGravityScale) + mForce) * deltaTime);
			particle.Position += (particle.Velocity * deltaTime);
			particle.Color = mColorInterpolator.GetValue(lifeRatio);
			particle.Size = glm::lerp<float>(5.0f, 0.0f, lifeRatio);
		}
		BuildVertexBuffer();
	}

	void ParticleEffect::Render(std::vector<std::shared_ptr<ParticleEffect>> multipleEffects)
	{
		glDisable(GL_DEPTH_TEST);           // Disables Depth Testing
		glEnable(GL_BLEND);                 // Enable Blending

		for (const auto& effect : multipleEffects)
		{
			switch (effect->mBlendingMode)
			{
			case BlendingMode::AlphaBlending:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case BlendingMode::AdditiveBlending:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			default:
				break;
			}

			glEnable(GL_TEXTURE_2D);            // Enable textures

			unsigned int VBO, VAO;
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, (effect->mVertices.size() * sizeof(Vertex)), effect->mVertices.data(), GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Diffuse)));

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Rotate)));

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, CameraRotation)));

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));

			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Size)));

			glDrawArrays(GL_POINTS, 0, effect->mVertices.size());
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	bool ParticleEffect::LoadTexture(const std::string & filename)
	{
		if (mTextureID != 0)
		{
			glDeleteTextures(1, &mTextureID);
		}

		uint32_t texture1;
		// texture 1
		// ---------
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

		//mTextureID = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
		return (mTextureID != 0);
	}

	void ParticleEffect::Resize(uint32_t particleCount)
	{
		mParticles.resize(particleCount, Particle());
		mVertices.resize(particleCount, Vertex());
	}

	void ParticleEffect::RandomizeParticle(Particle & particle)
	{
		particle.Age = 0.0f;
		particle.LifeTime = Random::RandRange(3, 5);

		glm::vec3 unitVec = Random::RandUnitVec();

		particle.Position = unitVec * 1.0f;
		particle.Velocity = unitVec * Random::RandRange(10, 20);
	}

	void ParticleEffect::EmitParticle(Particle & particle)
	{
		assert(mEmitter != nullptr);
		mEmitter->EmitParticle(particle);
	}

	void ParticleEffect::BuildVertexBuffer()
	{
		glm::quat cameraRotation;
		if (mCamera != nullptr)
		{
			cameraRotation = glm::quat(glm::radians(mCamera->GetRotation()));
		}
		mVertices.resize(mParticles.size(), Vertex());
		for (uint32_t i = 0; i < mParticles.size(); ++i)
		{
			Particle& particle = mParticles[i];
			glm::quat rotation = glm::angleAxis(particle.Rotate, glm::vec3(0.0f, 0.0f, 1.0f));
			Vertex& vertex = mVertices[i];
			vertex.Rotate = (rotation * particle.Size);
			vertex.CameraRotation = cameraRotation;
			vertex.Position = particle.Position;
			vertex.Diffuse = particle.Color;
			vertex.Size = particle.Size;
		}
	}

}