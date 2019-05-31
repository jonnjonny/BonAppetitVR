#ifndef CAVEAPP_H
#define CAVEAPP_H


#include "Core.h"
#include "RiftApp.hpp"
#include "Scene.hpp"
#include "Queue.h"


// An example application that renders a simple cube
class CaveApp : public RiftApp
{
	std::shared_ptr<Scene> scene;
	glm::mat4 view[3];
	glm::mat3 cachedRot[2];
	glm::vec4 cachedTrans[2];
	glm::vec3 cube_translation;
	float scaleFactor;
	float originalLeftEye;
	float originalRightEye;
	bool buttonPressed;
	bool triggerPressed;
	bool stickShifted;
	bool freeze;
	bool controllerView;
	bool debugMode;

public:
	CaveApp()
	{
		scaleFactor = 0.1f;
		buttonPressed = false;
		stickShifted = false;
		freeze = false;
		controllerView = false;
		debugMode = false;
		cube_translation = glm::vec3(0.0f);

	}

protected:
	void initGl() override
	{
		RiftApp::initGl();
		originalLeftEye = _viewScaleDesc.HmdToEyePose[0].Position.x;
		originalRightEye = _viewScaleDesc.HmdToEyePose[1].Position.x;
		float IOD = originalLeftEye - originalRightEye;
		
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		ovr_RecenterTrackingOrigin(_session);
		scene = std::shared_ptr<Scene>(new Scene());
	}

	void shutdownGl() override
	{
		scene.reset();
	}

	glm::mat4 buildTrackingMatrix(const glm::mat3 rot, const glm::vec4 translation) {

		glm::mat4 resultMatrix (1.0f);

		resultMatrix[0].x = rot[0].x;
		resultMatrix[0].y = rot[0].y;
		resultMatrix[0].z = rot[0].z;
		resultMatrix[1].x = rot[1].x;
		resultMatrix[1].y = rot[1].y;
		resultMatrix[1].z = rot[1].z;
		resultMatrix[2].x = rot[2].x;
		resultMatrix[2].y = rot[2].y;
		resultMatrix[2].z = rot[2].z;
		resultMatrix[3].x = translation[0];
		resultMatrix[3].y = translation[1];
		resultMatrix[3].z = translation[2];

		return resultMatrix;
	

	}

	void renderScene(GLuint _fbo, const glm::mat4& projectionLeft, const glm::mat4& headPoseLeft,
		const glm::mat4& projectionRight, const glm::mat4& headPoseRight, ovrEyeType eye) override
	{
		ovrInputState inputState;
		if (OVR_SUCCESS(ovr_GetInputState(_session, ovrControllerType_Touch, &inputState))) {
			if (inputState.Buttons & ovrButton_RThumb) {
				if (!buttonPressed) {
					scaleFactor = 0.1f;
					cube_translation = glm::vec3(0.0f);
					std::cout << "Cube Locations Reset and Size Reset To: " << int(scaleFactor * 200) << " cm" << std::endl;
					buttonPressed = true;
				}
			}
			else if (inputState.Buttons & ovrButton_B) {
				if (!buttonPressed) {
					freeze = !freeze;
					buttonPressed = true;
				}

			}
			else if (inputState.Thumbstick[ovrHand_Right].x > 0.5f) {
				if (!stickShifted && scaleFactor < 0.25001f) {
					scaleFactor += 0.005f;
					std::cout << "Cube Size Increased To: " << int(scaleFactor * 200) << " cm" << std::endl;
					stickShifted = true;
				}
			}
			else if (inputState.Thumbstick[ovrHand_Right].x < -0.5f) {
				if (!stickShifted && scaleFactor > 0.00501f) {
					scaleFactor -= 0.005f;
					std::cout << "Cube Size Decreased To: " << int(scaleFactor * 200) << " cm " << std::endl;
					stickShifted = true;
				}
			}
			else if (inputState.Thumbstick[ovrHand_Right].y > 0.5f) {
				if (!stickShifted) {
					cube_translation += glm::vec3(0.0f, 0.1f, 0.0f);
					stickShifted = true;
				}
			}
			else if (inputState.Thumbstick[ovrHand_Right].y < -0.5f) {
				if (!stickShifted) {
					cube_translation -= glm::vec3(0.0f, 0.1f, 0.0f);
					stickShifted = true;
				}
			}
			else if (inputState.Thumbstick[ovrHand_Left].x < -0.5f) {
				if (!stickShifted) {
					cube_translation -= glm::vec3(0.1f, 0.0f, 0.0f);
					stickShifted = true;
				}
			}
			else if (inputState.Thumbstick[ovrHand_Left].x > 0.5f) {
				if (!stickShifted) {
					cube_translation += glm::vec3(0.1f, 0.0f, 0.0f);
					stickShifted = true;
				}
			}
			else if (inputState.Thumbstick[ovrHand_Left].y < -0.5f) {
				if (!stickShifted) {
					cube_translation += glm::vec3(0.0f, 0.0f, 0.1f);
					stickShifted = true;
				}
			}
			else if (inputState.Thumbstick[ovrHand_Left].y > 0.5f) {
				if (!stickShifted) {
					cube_translation -= glm::vec3(0.0f, 0.0f, 0.1f);
					stickShifted = true;
				}
			}
			else {
				buttonPressed = false;
				stickShifted = false;
			}

			if (inputState.IndexTrigger[ovrHand_Right] > 0.5f) {
				controllerView = true;
			}
			else {
				controllerView = false;
			}

			if (inputState.Buttons & ovrButton_A) {
				debugMode = true;
			}
			else {
				debugMode = false;
			}
		}

		// get the general state hmdState
		double ftiming = ovr_GetPredictedDisplayTime(_session, 0);
		ovrTrackingState hmdState = ovr_GetTrackingState(_session,
			ftiming, ovrTrue);

		// Get the state of hand poses
		ovrPoseStatef handPoseState = hmdState.HandPoses[ovrHand_Right];
		//Get the hand pose position.
		glm::vec3 controllerPosition = ovr::toGlm(
			handPoseState.ThePose.Position);
		glm::mat4 controller_transform =
			glm::translate(glm::mat4(1.0f), controllerPosition) * glm::mat4_cast(ovr::toGlm(handPoseState.ThePose.Orientation));
		glm::mat4 headPose_L = headPoseLeft;
		glm::mat4 headPose_R = headPoseRight;


		if (eye == ovrEye_Left) {
			scene->render(_fbo, projectionLeft, glm::inverse(headPose_L), eye, cube_translation, scaleFactor, freeze, controllerView, debugMode, controller_transform);
		}
		else {
			scene->render(_fbo, projectionRight, glm::inverse(headPose_R), eye, cube_translation, scaleFactor, freeze, controllerView, debugMode, controller_transform);
		}
		
		
	}
};


#endif //CAVEAPP_H
