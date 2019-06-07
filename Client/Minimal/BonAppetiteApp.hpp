#ifndef BONAPPETITEAPP_H
#define BONAPPETITEAPP_H


#include "Core.h"
#include "RiftApp.hpp"
#include "Scene.hpp"
#include "SceneGraph.hpp"
#include "ObjectData.hpp"
#include "BoundingBox.hpp"
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
		InitialData initialData = scene->getInitialData();
		c->call("loadInitialData", initialData);
	}

	void shutdownGl() override
	{
		scene.reset();
	}

	PlayerData getPlayerState(ovrTrackingState hmdState, int playerNumber) {
		PlayerData output;
		ovrPoseStatef headPoseState = hmdState.HeadPose;
		output.headPos = ovr::toGlm(headPoseState.ThePose.Position);
		output.headOri = ovr::toGlm(headPoseState.ThePose.Orientation);

		ovrPoseStatef handPoseStateL = hmdState.HandPoses[ovrHand_Left];
		ovrPoseStatef handPoseStateR = hmdState.HandPoses[ovrHand_Right];
		output.LControlPos = ovr::toGlm(handPoseStateL.ThePose.Position);
		output.LControlOri = ovr::toGlm(handPoseStateL.ThePose.Orientation);
		output.RControlPos = ovr::toGlm(handPoseStateR.ThePose.Position);
		output.RControlOri = ovr::toGlm(handPoseStateR.ThePose.Orientation);
		output.xmin = output.ymin = output.zmin = -0.01;
		output.xmax = output.ymax = output.zmax = 0.01;

		if (playerNumber == 1) {
			output.headPos = glm::vec3(2.0,0.0,0.0) + output.headPos;
			output.LControlPos = glm::vec3(2.0, 0.0, 0.0) + output.LControlPos;
			output.RControlPos = glm::vec3(2.0, 0.0, 0.0) + output.RControlPos;
		}

		return output;
	}


	void renderScene(const glm::mat4& projection, const glm::mat4& eyePose) override
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
			
		SceneGraph scenegraph = c->call("updatePlayer", getPlayerState(hmdState,playerNumber), playerNumber).as<SceneGraph>();

		scene->update(scenegraph);
	
		scene->render(projection, playerNumber == 0 ? glm::inverse(eyePose) : glm::inverse(glm::translate(glm::mat4(1.0),glm::vec3(2.0,0.0,0.0)) * eyePose), playerNumber);
		
		
		
	}
};


#endif //BONAPPETITEAPP_H
