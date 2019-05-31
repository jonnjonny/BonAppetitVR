#ifndef BONAPPETITEAPP_H
#define BONAPPETITEAPP_H


#include "Core.h"
#include "RiftApp.hpp"
#include "Scene.hpp"

// An example application that renders a simple cube
class BonAppetiteApp : public RiftApp
{
	std::shared_ptr<Scene> scene;
	bool buttonPressed;
	bool stickShifted;

public:
	BonAppetiteApp()
	{
		buttonPressed = false;
		stickShifted = false;
	}

protected:
	void initGl() override
	{
		RiftApp::initGl();
		
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		ovr_RecenterTrackingOrigin(_session);
		scene = std::shared_ptr<Scene>(new Scene());
	}

	void shutdownGl() override
	{
		scene.reset();
	}


	void renderScene(const glm::mat4& projection, const glm::mat4& headPose) override
	{
		ovrInputState inputState;
		if (OVR_SUCCESS(ovr_GetInputState(_session, ovrControllerType_Touch, &inputState))) {
			if (inputState.Buttons & ovrButton_RThumb) {
				if (!buttonPressed) {
					buttonPressed = true;
				}
			}
			else if (inputState.Buttons & ovrButton_B) {
				if (!buttonPressed) {
					buttonPressed = true;
				}

			}
			else if (inputState.Thumbstick[ovrHand_Right].x > 0.5f) {
				if (!buttonPressed) {
					buttonPressed = true;
				}
			}
			else if (inputState.Thumbstick[ovrHand_Right].x < -0.5f) {
				if (!buttonPressed) {
					buttonPressed = true;
				}
			}
			else if (inputState.Thumbstick[ovrHand_Right].y > 0.5f) {
				if (!buttonPressed) {
					buttonPressed = true;
				}
			}
			else if (inputState.Thumbstick[ovrHand_Right].y < -0.5f) {
				if (!buttonPressed) {
					buttonPressed = true;
				}
			}
			else if (inputState.Thumbstick[ovrHand_Left].x < -0.5f) {
				if (!buttonPressed) {
					buttonPressed = true;
				}
			}
			else if (inputState.Thumbstick[ovrHand_Left].x > 0.5f) {
				if (!buttonPressed) {
					buttonPressed = true;
				}
			}
			else if (inputState.Thumbstick[ovrHand_Left].y < -0.5f) {
				if (!buttonPressed) {
					buttonPressed = true;
				}
			}
			else if (inputState.Thumbstick[ovrHand_Left].y > 0.5f) {
				if (!buttonPressed) {
					buttonPressed = true;
				}
			}
			else {
				buttonPressed = false;
				stickShifted = false;
			}
		}


	
		scene->render(projection, glm::inverse(headPose));
		
		
		
	}
};


#endif //BONAPPETITEAPP_H
