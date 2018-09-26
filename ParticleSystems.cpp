/*
This project was made solely for educational purposes and is designed to be used as a standalone only
Thanks to LearnOpenGL, for providing a base with a lot of the boiler plate code.
Used ImGUI for a GUI

Features:
User can see a couple demo effets namely:
	Tornado effect(I tried my best but I'm no artist)
	Box and sphere surface demo, to demonstarte surface emission
Users can also create a new effect with all the parameters they want,
	Number of particles
	Different types of emitters
	2 emission modes, burst and continuous
	Overlife functions that use lifetime of the effect for changing the property, 0 (birth) to 1 (death)
		Color over life values, provide a default value
		Velocity over life, provide a default value as well
		Force over life, no need to provide default value
	Gravity scale
GUI allows customization of the effect.
Mouse left-click and WASD to control the camera.

*/

#include "pch.h"
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <time.h>
#include <imgui.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ParticleEffect.h"
#include "PointEmitter.h"
#include "PlaneEmitter.h"
#include "SphereEmitter.h"

using namespace ParticleSystem;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 155.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Particle Systems", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);


	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::vector<std::shared_ptr<ParticleEffect>> effectsToUpdate;

	ParticleEffect::VelocityInterpolator surfaceVelocity;
	surfaceVelocity.AddValue(0.00f, 0.0f);		// 5% velocity
	surfaceVelocity.AddValue(1.00f, 0.0f);		// 5% velocity

	ParticleEffect PointEffect1(1000);
	ParticleEffect PointEffect2(1000);

	ParticleEffect BoxEffect(10000);
	ParticleEffect SphereEffect(1000);

	//point emitter effect
	PointEmitter smokeEmitter1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-10.0f, 0.0f, 10.0f));
	smokeEmitter1.SetParticleLifetimeRange(1.0f, 5.0f);
	PointEmitter smokeEmitter2(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 0.0f, -10.0f));
	smokeEmitter2.SetParticleLifetimeRange(1.0f, 5.0f);

	ParticleEffect::ColorInterpolator smokeColors;
	smokeColors.AddValue(0.00f, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	smokeColors.AddValue(0.25f, glm::vec4(0.3f, 0.3f, 0.3f, 0.3f));
	smokeColors.AddValue(1.00f, glm::vec4(0.4f, 0.4f, 0.4f, 0.4f));

	ParticleEffect::VelocityInterpolator pointVelocity;
	pointVelocity.AddValue(0.00f, 0.8f);		// 5% velocity
	pointVelocity.AddValue(1.00f, 0.8f);		// 5% velocity

	ParticleEffect::ForceInterpolator centrifugalForce1;
	centrifugalForce1.AddValue(0.00f, glm::vec3(-100.0f, 100.0f, 100.0f));
	centrifugalForce1.AddValue(0.15f, glm::vec3(-150.0f, 150.0f, -150.0f));
	centrifugalForce1.AddValue(0.40f, glm::vec3(200.0f, 200.0f, -200.0f));
	centrifugalForce1.AddValue(0.80f, glm::vec3(250.0f, 250.0f, 250.0f));
	centrifugalForce1.AddValue(1.00f, glm::vec3(0.0f, 0.0f, 0.0f));
	ParticleEffect::ForceInterpolator centrifugalForce2;
	centrifugalForce2.AddValue(0.00f, glm::vec3(100.0f, 100.0f, -100.0f));
	centrifugalForce2.AddValue(0.15f, glm::vec3(150.0f, 150.0f, 150.0f));
	centrifugalForce2.AddValue(0.40f, glm::vec3(-200.0f, 200.0f, 200.0f));
	centrifugalForce2.AddValue(0.80f, glm::vec3(-250.0f, 250.0f, -250.0f));
	centrifugalForce2.AddValue(1.00f, glm::vec3(0.0f, 0.0f, 0.0f));

	PointEffect1.InitializeEffect(window, &camera, &smokeEmitter1);
	PointEffect1.SetEffectParameters(&smokeColors, &pointVelocity, &centrifugalForce1);
	PointEffect1.SetGravityScale(0.0f);
	PointEffect2.InitializeEffect(window, &camera, &smokeEmitter2);
	PointEffect2.SetEffectParameters(&smokeColors, &pointVelocity, &centrifugalForce2);
	PointEffect2.SetGravityScale(0.0f);

	//effectsToUpdate.push_back(std::make_shared<ParticleEffect>(PointEffect1));
	//effectsToUpdate.push_back(std::make_shared<ParticleEffect>(PointEffect2));

	//box emitter effect
	BoxEmitter planeEmimtter(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(200.0f, 200.0f, 50.0f));
	planeEmimtter.SetEmissionType(EmissionType::Surface);
	planeEmimtter.SetParticleLifetimeRange(2.0f, 5.0f);

	ParticleEffect::ColorInterpolator boxColors;
	boxColors.AddValue(0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0.8f));		// white
	boxColors.AddValue(0.25f, glm::vec4(0.85f, 0.30f, 0.15f, 0.6f));   // yellow
	boxColors.AddValue(0.5f, glm::vec4(0.98f, 0.45f, 0.05f, 0.5f));    // dark orange

	ParticleEffect::ForceInterpolator boxForce;
	boxForce.AddValue(0.00f, glm::vec3(0.0f, 0.0f, 0.0f));
	boxForce.AddValue(1.00f, glm::vec3(0.0f, 0.0f, 0.0f));

	BoxEffect.InitializeEffect(window, &camera, &planeEmimtter);
	BoxEffect.SetEffectParameters(&boxColors, &surfaceVelocity, &boxForce);
	BoxEffect.SetGravityScale(1.0f);

	//effectsToUpdate.push_back(std::make_shared<ParticleEffect>(BoxEffect));

	//sphere emitter effect
	SphereEmitter sphereEmitter(glm::vec3(0.0f, 0.0f, 0.0f), 50.0f);
	sphereEmitter.SetParticleLifetimeRange(2.0f, 5.0f);
	sphereEmitter.SetEmissionType(EmissionType::Surface);

	ParticleEffect::ColorInterpolator sphereColors;
	sphereColors.AddValue(0.00f, glm::vec4(0.0f, 0.1f, 0.1f, 1.0f));
	sphereColors.AddValue(0.25f, glm::vec4(0.0f, 0.2f, 0.2f, 1.0f));
	sphereColors.AddValue(0.50f, glm::vec4(0.0f, 0.2f, 0.5f, 1.0f));

	ParticleEffect::ForceInterpolator sphereForce;
	sphereForce.AddValue(0.00f, glm::vec3(0.0f, 0.0f, 0.0f));
	sphereForce.AddValue(1.00f, glm::vec3(0.0f, 0.0f, 0.0f));

	SphereEffect.InitializeEffect(window, &camera, &sphereEmitter);
	SphereEffect.SetEffectParameters(&sphereColors, &surfaceVelocity, &sphereForce);
	SphereEffect.SetGravityScale(0.0f);

	//effectsToUpdate.push_back(std::make_shared<ParticleEffect>(SphereEffect));



	Shader shader("Content/Shaders/vertex_shader.vs", "Content/Shaders/fragment_shader.fs", "Content/Shaders/geometry_shader.gs");

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
	unsigned char *data = stbi_load("Content\\Textures\\fire_smoke_texture2.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	shader.use();
	shader.setInt("texture1", 0);
	//srand(time(nullptr));

	bool bVolumeEmission = true;
	bool bSurfaceEmission = false;

	bool bContinuousEmission = true;
	bool bBurstEmission = false;

	float tempPosition[3] = { 0.0f, 0.0f, 0.0f };
	float tempSize[3] = {0.0f, 0.0f, 0.0f};
	float emitterRadius = 0.0f;
	glm::vec3 emitterOrigin;
	glm::vec3 emitterSize;
	float minLifetime = 0.0f;
	float maxLifetime = 0.0f;
	int particleCount = 0;

	bool bPointEmitter = true;
	bool bBoxEmitter = false;
	bool bSphereEmitter = false;

	bool bCreateEffect = false;

	bool bTorando = true;
	bool bBoxSurface = false;
	bool bSphereSurface = false;
	bool dirtyFlag = false;

	ParticleEffect userEffect;
	BoxEmitter box;
	SphereEmitter sphere;
	PointEmitter point;

	ParticleEffect::ColorInterpolator colors;
	ParticleEffect::VelocityInterpolator velocityOverLife;
	ParticleEffect::ForceInterpolator forceOverLife;

	float tempColors[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float colorTime = 0.0f;

	float tempVelocity = 0.0f;
	float velocityTime = 0.0f;
	
	float tempGravityScale = 0.0f;
	
	float tempForces[3] = { 0.0f, 0.0f, 0.0f };
	float forceTime = 0.0f;

	float azimuthRange[2] = { 0.0f, 360.0f };
	float altitudeRange[2] = { 0.0f, 180.0f };


	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float fps = 1.0f / deltaTime;
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//if (show_demo_window)
		//	ImGui::ShowDemoWindow(&show_demo_window);

		{
			ImGui::Begin("Particle System Maker!");
			if (ImGui::RadioButton("Use Existing Effects", !bCreateEffect))
			{
				bCreateEffect = false;
			}
			if (ImGui::RadioButton("Create new effect", bCreateEffect))
			{
				bCreateEffect = true;
			}

			if (!bCreateEffect)
			{
				ImGui::Text("\n");
				ImGui::Text("Select Demo");
				if (ImGui::RadioButton("Tornado demo", bTorando))
				{
					dirtyFlag = true;

					bTorando = true;
					bBoxSurface = false;
					bSphereSurface = false;
				}
				if (ImGui::RadioButton("Box surface demo", bBoxSurface))
				{
					dirtyFlag = true;

					bTorando = false;
					bBoxSurface = true;
					bSphereSurface = false;
				}
				if (ImGui::RadioButton("Sphere surface demo", bSphereSurface))
				{
					dirtyFlag = true;

					bTorando = false;
					bBoxSurface = false;
					bSphereSurface = true;
				}

				if (bTorando && dirtyFlag)
				{
					effectsToUpdate.clear();
					effectsToUpdate.push_back(std::make_shared<ParticleEffect>(PointEffect1));
					effectsToUpdate.push_back(std::make_shared<ParticleEffect>(PointEffect2));
					dirtyFlag = false;
				}
				else if (bBoxSurface && dirtyFlag)
				{
					effectsToUpdate.clear();
					effectsToUpdate.push_back(std::make_shared<ParticleEffect>(BoxEffect));
					dirtyFlag = false;
				}
				else if (bSphereSurface && dirtyFlag)
				{
					effectsToUpdate.clear();
					effectsToUpdate.push_back(std::make_shared<ParticleEffect>(SphereEffect));
					dirtyFlag = false;
				}
			}
			else
			{
				//Particle Count
				{
					ImGui::InputInt("Number of Particles", &particleCount);
				}

				//Emitter type
				{
					ImGui::Text("\n");
					ImGui::Text("Select emitter type");                          // Create a window called "Hello, world!" and append into it.
					if (ImGui::RadioButton("Point emitter", bPointEmitter))
					{
						bPointEmitter = true;
						bBoxEmitter = false;
						bSphereEmitter = false;
					}
					if (ImGui::RadioButton("Box emitter", bBoxEmitter))
					{
						bPointEmitter = false;
						bBoxEmitter = true;
						bSphereEmitter = false;
					}
					if (ImGui::RadioButton("Sphere emitter", bSphereEmitter))
					{
						bPointEmitter = false;
						bBoxEmitter = false;
						bSphereEmitter = true;
					}
				}

				//Emission type
				if (bBoxEmitter || bSphereEmitter)
				{
					ImGui::Text("Select Emission Type");
					if (ImGui::RadioButton("Volume Emission", bVolumeEmission))
					{
						bVolumeEmission = true;
						bSurfaceEmission = false;
						sphere.SetEmissionType(EmissionType::Volume);
						box.SetEmissionType(EmissionType::Volume);
					}
					if (ImGui::RadioButton("Surface Emission", bSurfaceEmission))
					{
						bVolumeEmission = false;
						bSurfaceEmission = true;
						sphere.SetEmissionType(EmissionType::Surface);
						box.SetEmissionType(EmissionType::Surface);
					}
				}

				if (bSphereEmitter)
				{
					ImGui::Text("\n");
					ImGui::Text("Set Azimuth and Altitude");
					ImGui::InputFloat2("Azimuth Range", azimuthRange);
					ImGui::InputFloat2("Altitude Range", altitudeRange);
					if (ImGui::Button("Set values"))
					{
						sphere.SetAzimuthRange(azimuthRange[0], azimuthRange[1]);
						sphere.SetAltitudeRange(altitudeRange[0], altitudeRange[1]);
					}
				}
				
				//Emitter origin and size
				{
					ImGui::Text("\n");
					ImGui::Text("Add emitter parameters");
					ImGui::InputFloat3("Emitter origin", tempPosition);
					emitterOrigin = { tempPosition[0], tempPosition[1], tempPosition[2] };

					if (bBoxEmitter)
					{
						ImGui::InputFloat3("Emitter Size", tempSize);
						emitterSize = { tempSize[0], tempSize[1], tempSize[2] };
					}
					if (bSphereEmitter)
					{
						ImGui::InputFloat("Emitter Size", &emitterRadius);
					}
				}

				//Emission mode, continuous or burst
				{
					ImGui::Text("\n");
					ImGui::Text("Select mode of emission");
					if (ImGui::RadioButton("Burst Emission", bBurstEmission))
					{
						bContinuousEmission = false;
						bBurstEmission = true;
					}
					if (ImGui::RadioButton("Continuous Emission", bContinuousEmission))
					{
						bContinuousEmission = true;
						bBurstEmission = false;
					}
					if (bContinuousEmission)
					{
						ImGui::Text("Particle Lifetime:");
						ImGui::InputFloat("Minimum lifetime", &minLifetime);
						ImGui::InputFloat("Maximum lifetime", &maxLifetime);
					}
					if (bBurstEmission)
					{
						ImGui::InputFloat("Frequency of emission", &minLifetime);
						maxLifetime = minLifetime;
					}

					if (bBoxEmitter)
					{
						box.SetParticleLifetimeRange(minLifetime, maxLifetime);
					}
					else if (bSphereEmitter)
					{
						sphere.SetParticleLifetimeRange(minLifetime, maxLifetime);
					}
					else if (bPointEmitter)
					{
						point.SetParticleLifetimeRange(minLifetime, maxLifetime);
					}
				}

				if (ImGui::Button("Refresh Emitter"))
				{
					if (bPointEmitter)
					{
						point.SetPosition(emitterOrigin);
					}
					else if (bBoxEmitter)
					{
						box.SetPosition(emitterOrigin);
						box.SetSize(emitterSize);
					}
					else if (bSphereEmitter)
					{
						sphere.SetPosition(emitterOrigin);
						sphere.SetRadius(emitterRadius);
					}
				}

				{
					ImGui::Text("\n");
					ImGui::Text("Set color over life");
					ImGui::InputFloat("Time point for color", &colorTime);
					ImGui::InputFloat4("Color-over-life value", tempColors);
					if (ImGui::Button("Add Color"))
					{
						colors.AddValue(colorTime, glm::vec4(tempColors[0], tempColors[1], tempColors[2], tempColors[3]));
					}
					if (ImGui::Button("Clear Colors"))
					{
						colors.Clear();
					}

					ImGui::Text("\n");
					ImGui::Text("Set velocity over life");
					ImGui::InputFloat("Time point for velocity", &velocityTime);
					ImGui::InputFloat("Velocity-over-life value", &tempVelocity);
					if (ImGui::Button("Add Velocity"))
					{
						velocityOverLife.AddValue(velocityTime, tempVelocity);
					}
					if (ImGui::Button("Clear Velocities"))
					{
						velocityOverLife.Clear();
					}
					
					ImGui::Text("\n");
					ImGui::Text("set force over life");
					ImGui::InputFloat("Time point for force", &forceTime);
					ImGui::InputFloat4("Force-over-life value", tempForces);
					if (ImGui::Button("Add Force"))
					{
						forceOverLife.AddValue(colorTime, glm::vec3(tempForces[0], tempForces[1], tempForces[2]));
					}
					if (ImGui::Button("Clear Forces"))
					{
						forceOverLife.Clear();
					}
					
					ImGui::Text("\n");
					ImGui::InputFloat("Set gravity scale", &tempGravityScale);
					if (ImGui::Button("Set gravity"))
					{
						userEffect.SetGravityScale(tempGravityScale);
					}
				}


				if (ImGui::Button("Create New Effect"))
				{
					userEffect.Resize(particleCount);

					if (bPointEmitter)
					{
						userEffect.InitializeEffect(window, &camera, &point);
					}
					else if (bBoxEmitter)
					{
						userEffect.InitializeEffect(window, &camera, &box);
					}
					else if (bSphereEmitter)
					{
						userEffect.InitializeEffect(window, &camera, &sphere);
					}

					userEffect.SetEffectParameters(&colors, &velocityOverLife, &forceOverLife);

					effectsToUpdate.clear();
					effectsToUpdate.push_back(std::make_shared<ParticleEffect>(userEffect));
				}
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		shader.use();

		// configure transformation matrices
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000000.0f);
		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model;

		glm::mat4 viewProjection = projection * view * model;
		shader.setMat4("viewProjection", viewProjection);

		for (const auto& effect : effectsToUpdate)
		{
			effect->Update(deltaTime);
		}

		ParticleEffect::Render(effectsToUpdate);
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		//glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	else
	{
		firstMouse = true;
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
