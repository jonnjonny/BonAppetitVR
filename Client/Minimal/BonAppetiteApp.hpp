#ifndef BONAPPETITEAPP_H
#define BONAPPETITEAPP_H


#include "Core.h"
#include "RiftApp.hpp"
#include "Scene.hpp"
#include "rpc/client.h"

// An example application that renders a simple cube
class BonAppetiteApp : public RiftApp
{
	std::shared_ptr<Scene> scene;
	rpc::client* c;
	int playerNumber;
	bool buttonPressed;
	bool stickShifted;

public:
	BonAppetiteApp(rpc::client* connection)
	{
		buttonPressed = false;
		stickShifted = false;
		c = connection;
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

		double ftiming = ovr_GetPredictedDisplayTime(_session, 0);
		ovrTrackingState hmdState = ovr_GetTrackingState(_session,
			ftiming, ovrTrue);


		auto[scene]/* structured binding: similar to
									result = std::get<0>(return value);
									player = std::get<1>(return value);
								*/ =
			c->call("updatePlayer", headPose, hmdState, playerNumber ).as<std::tuple<glm::mat4, ovrTrackingState, int>/*cast back the respond message to string and Player*/>();


	
		scene->render(projection, glm::inverse(headPose));
		
		
		
	}
};


#endif //BONAPPETITEAPP_H
