#ifndef BONAPPETITEAPP_H
#define BONAPPETITEAPP_H


#include "Core.h"
#include "RiftApp.hpp"
#include "Scene.hpp"
#include "SceneGraph.hpp"
#include "ObjectData.hpp"
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
		playerNumber = c->call("getPlayerNumber").as<int>();
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


		 
			
		SceneGraph scenegraph = c->call("updatePlayer",  ).as<SceneGraph>/*cast back the respond message to string and Player*/>();

		scene->updatePlayer(scenegraph.player1, 0);
		scene->updatePlayer(scenegraph.player2, 1);

	
		scene->render(projection, glm::inverse(headPose));
		
		
		
	}

	PlayerData getPlayerState(glm::mat4 headPose, ovrTrackingState handState) {
		PlayerData output;
		output.headPos = headPose[3];
		output.headOri = glm::mat3(headPose);
		output.LControlPos = glm::vec3(leftControllerPosition * glm::vec4(0.0, 0.0, 0.0, 1.0));
		output.LControlOri = glm::quat_cast(leftControllerOrientation);
		output.RControlPos = glm::vec3(rightControllerPosition * glm::vec4(0.0, 0.0, 0.0, 1.0));
		output.RControlOri = glm::quat_cast(rightControllerOrientation);

		return output;
	}
};


#endif //BONAPPETITEAPP_H
