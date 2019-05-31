//
// Created by Jonathan Leung May 2019.
//

#ifndef CAVE_SCENE_H
#define CAVE_SCENE_H


#include <vector>
#include "Core.h"
#include "shader.h"
#include "Cube.h"
#include "TexturedCube.h"
#include "Screen.h"
#include "Skybox.h"
#include "Model.h"
#include "Pyramid.h"

// a class for building and rendering cubes
class Scene
{
	// Program
	std::vector<glm::mat4> instance_positions;
	GLuint instanceCount;
	GLuint shaderID, screenShaderID, sphereShaderID, pyramidShaderID;

	Model* sphere;
	std::unique_ptr<TexturedCube> cube;
	std::vector<Screen*> screen;
	std::vector<Pyramid*> pyramid;
	glm::mat4 screenPose1, screenPose2, screenPose3;
	std::vector<glm::mat4> P_prime_left, P_prime_right;
	std::unique_ptr<Skybox> skyboxLeft;
	std::unique_ptr<Skybox> skyboxRight;
	glm::vec3 leftEye_translation, rightEye_translation;
	

	const unsigned int GRID_SIZE{ 5 };

public:
	Scene()
	{
		// Create two cube
		instance_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.3)));
		instance_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.9)));
		screenPose1 = glm::translate(glm::mat4(1.0f), glm::vec3(-1.2f*glm::cos(glm::radians(45.0f)), 0, -0.2));
		screenPose2 = glm::translate(glm::mat4(1.0f), glm::vec3(1.2f*glm::cos(glm::radians(45.0f)), 0, -0.2));
		screenPose3 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -1.2f, -0.2 + (1.2f / glm::sqrt(2.0f))));

		instanceCount = instance_positions.size();

		// Shader Program
		shaderID = LoadShaders("skybox.vert", "skybox.frag");

		screenShaderID = LoadShaders("screen.vert", "screen.frag");

		sphereShaderID = LoadShaders("sphere.vert", "sphere.frag");

		pyramidShaderID = LoadShaders("pyramid.vert", "pyramid.frag");

		//Create Skybox

		cube = std::make_unique<TexturedCube>("cube");

		//Create Screens
		Screen* screen1 = new Screen();
		Screen* screen2 = new Screen();
		Screen* screen3 = new Screen();

		screen1->toWorld = screenPose1 * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.2f));
		screen2->toWorld = screenPose2 * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.2f));
		screen3->toWorld = screenPose3 * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.2f));

		screen1->determineCorners();
		screen2->determineCorners();
		screen3->determineCorners();

		screen.push_back(screen1);
		screen.push_back(screen2);
		screen.push_back(screen3);

		//Creating Pyramids
		for (int i = 0; i < 3; i++) {
			std::vector<float> vert;

			glm::vec3 bottomLeft = screen.at(i)->bottomLeft;
			glm::vec3 bottomRight = screen.at(i)->bottomRight;
			glm::vec3 topLeft = screen.at(i)->topLeft;
			glm::vec3 topRight = screen.at(i)->topRight;

			vert.push_back(0.0f);
			vert.push_back(0.0f);
			vert.push_back(0.0f);
			vert.push_back(bottomLeft.x);
			vert.push_back(bottomLeft.y);
			vert.push_back(bottomLeft.z);
			vert.push_back(topLeft.x);
			vert.push_back(topLeft.y);
			vert.push_back(topLeft.z);
			vert.push_back(topRight.x);
			vert.push_back(topRight.y);
			vert.push_back(topRight.z);
			vert.push_back(bottomRight.x);
			vert.push_back(bottomRight.y);
			vert.push_back(bottomRight.z);

			Pyramid* pyr = new Pyramid(vert);
			pyramid.push_back(pyr);
		}

		for (int i = 0; i < 3; i++) {
			P_prime_left.push_back(glm::mat4(0.0f));
			P_prime_right.push_back(glm::mat4(0.0f));
		}

		sphere = new Model("sphere.obj");

		// 10m wide sky box for left eye
		skyboxLeft = std::make_unique<Skybox>("skybox");
		skyboxLeft->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));

		// 10m wide sky box for right eye
		skyboxRight = std::make_unique<Skybox>("skybox_righteye");
		skyboxRight->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));

		leftEye_translation = glm::vec3(-0.0325f, 0.0f, 0.0f);
		rightEye_translation = glm::vec3(0.0325f, 0.0f, 0.0f);

	}

	void render(const GLuint _fbo, const glm::mat4& projection, const glm::mat4& view, const ovrEyeType eye,
		const glm::vec3 cube_translation, const float scaleFactor, const bool freeze, const bool controllerView, const bool debugMode, const glm::mat4 &controller_transform)
	{
		glm::mat4 leftEyeTransform = (controller_transform*glm::translate(glm::mat4(1.0f), leftEye_translation));
		glm::mat4 rightEyeTransform = (controller_transform*glm::translate(glm::mat4(1.0f), rightEye_translation));

		glm::vec3 leftEyePos = glm::vec3(leftEyeTransform * vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glm::vec3 rightEyePos = glm::vec3(rightEyeTransform * vec4(0.0f, 0.0f, 0.0f, 1.0f));

		glm::vec3 eyePos;
		
		if (controllerView) {
			if (eye == ovrEye_Left) eyePos = leftEyePos;
			else eyePos = rightEyePos;
		}
		else {
			eyePos = glm::vec3(glm::inverse(view) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		}

		for (int i = 0; i < 3; i++) {

			GLint viewportDims[4];
			glGetIntegerv(GL_VIEWPORT, viewportDims);

			glViewport(0, 0, 4096, 4096);


			glBindFramebuffer(GL_FRAMEBUFFER, screen.at(i)->fbo);


			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 P_prime;

			if (freeze) {
				if (eye == ovrEye_Left) P_prime = P_prime_left.at(i);
				else P_prime = P_prime_right.at(i);
			}
			else {
				glm::vec3 pa = screen.at(i)->bottomLeft;
				glm::vec3 pb = screen.at(i)->bottomRight;
				glm::vec3 pc = screen.at(i)->topLeft;

				glm::vec3 vr = glm::normalize(pb - pa);
				glm::vec3 vu = glm::normalize(pc - pa);
				glm::vec3 vn = glm::normalize(glm::cross(vr, vu));

				glm::vec3 va = pa - eyePos;
				glm::vec3 vb = pb - eyePos;
				glm::vec3 vc = pc - eyePos;

				float d = -glm::dot(vn, va);
				float n = 0.01f;
				float f = 20.0f;

				float l = (glm::dot(vr, va)*n) / d;
				float b = (glm::dot(vu, va)*n) / d;
				float r = (glm::dot(vr, vb)*n) / d;
				float t = (glm::dot(vu, vc)*n) / d;

				glm::mat4 P = glm::frustum(l, r, b, t, n, f);
				glm::mat4 M = glm::mat4(glm::vec4(vr, 0.0f), glm::vec4(vu, 0.0f), glm::vec4(vn, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
				glm::mat4 T = glm::translate(glm::mat4(1.0f), -eyePos);

				P_prime = P * glm::transpose(M) * T;

				if (eye == ovrEye_Left) {
					P_prime_left.at(i) = P_prime;
				}
				else {
					P_prime_right.at(i) = P_prime;
				}
			}


			// Render two cubes
			for (int j = 0; j < 2; j++)
			{
				cube->toWorld = glm::translate(glm::mat4(1.0f),cube_translation) * instance_positions[j] * glm::scale(glm::mat4(1.0f),glm::vec3(scaleFactor));
				cube->draw(shaderID, P_prime, glm::mat4(1.0f));
			}

			// Render Skybox : remove view translation

			if (eye == ovrEye_Left) skyboxLeft->draw(shaderID, P_prime, glm::mat4(1.0f));
			else skyboxRight->draw(shaderID, P_prime, glm::mat4(1.0f));


			glViewport(viewportDims[0], viewportDims[1], viewportDims[2], viewportDims[3]);
			glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

			screen.at(i)->draw(screenShaderID, projection, view);

		}

		if (controllerView && debugMode) {
			for (int k = 0; k < 6; k++) {
				if (k % 2 == 0) {
					pyramid.at(k/2)->draw(pyramidShaderID, projection, view, leftEyePos,1);
				}
				else {
					pyramid.at(k/2)->draw(pyramidShaderID, projection, view, rightEyePos,0);
				}
			}
		}

		sphere->Draw(sphereShaderID, projection, view, leftEyeTransform*glm::scale(glm::mat4(1.0f),glm::vec3(0.01f)),1);
		sphere->Draw(sphereShaderID, projection, view, rightEyeTransform*glm::scale(glm::mat4(1.0f),glm::vec3(0.01f)),0);
	}


};


#endif //CAVE_SCENE_H
