#pragma once
#include "Emitter.h"
#include "Interpolator.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

class Camera;

namespace ParticleSystem
{
	class IEmitter;

	enum class BlendingMode
	{
		AlphaBlending,
		AdditiveBlending
	};

	struct Vertex
	{
		glm::vec4 Diffuse;
		glm::quat Rotate;
		glm::quat CameraRotation;
		glm::vec3 Position;
		float Size;
		//glm::vec2 TextureCoordinate;

		Vertex()
			: Position(0), Diffuse(0) //TextureCoordinate(0)
		{}
	};

	class ParticleEffect
	{
	public:
		using VertexBuffer = std::vector<Vertex>;
		using ColorInterpolator = Interpolator<glm::vec4>;
		using VelocityInterpolator = Interpolator<float>;
		using ForceInterpolator = Interpolator<glm::vec3>;

		ParticleEffect(uint32_t particleCount = 0);
		~ParticleEffect();

		void SetWindow(GLFWwindow* window);
		void SetCamera(Camera* camera);
		void SetEmitter(IEmitter* emitter);
		void SetColorInterpolator(const ColorInterpolator& color);
		void SetVelocityInterpolator(const VelocityInterpolator& velocity);
		void SetForceInterpolator(const ForceInterpolator& force);
		void SetGravityScale(float scale);
		void SetForce(glm::vec3 force);
		void SetBlendingMode(BlendingMode blendMode);

		void InitializeEffect(GLFWwindow* window, Camera* camera, IEmitter* emitter);
		void InitializeEffect(GLFWwindow* window, Camera* camera, IEmitter* emitter, glm::vec3 initialVelocity);
		void SetEffectParameters(ColorInterpolator* colorInterpolator = nullptr, VelocityInterpolator* velocityInterpolator = nullptr, ForceInterpolator* forceInterpolator = nullptr, BlendingMode blend = BlendingMode::AdditiveBlending);
		void RandomizeParticles();
		void EmitParticles();

		void Update(float deltaTime);
		static void Render(std::vector<std::shared_ptr<ParticleEffect>> multipleEffects);

		bool LoadTexture(const std::string& filename);
		void Resize(uint32_t particleCount);

	private:
		void RandomizeParticle(Particle& particle);
		void EmitParticle(Particle& particle);
		void BuildVertexBuffer();

		//Variables
		GLFWwindow* mWindow;
		Camera* mCamera;
		IEmitter* mEmitter;
		ColorInterpolator mColorInterpolator;
		VelocityInterpolator mVelocityInterpolator;
		ForceInterpolator mForceInterpolator;
		glm::vec3 mForce;
		glm::vec3 mGravity;
		float mGravityScale = 1.0f;
		BlendingMode mBlendingMode = BlendingMode::AlphaBlending;

		ParticleBuffer mParticles;
		VertexBuffer mVertices;
		glm::mat4x4 mWorldTransformMatrix;
		uint32_t mTextureID;
	};
}
