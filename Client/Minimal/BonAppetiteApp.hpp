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
	bool leftIndexTrigger;
	bool rightIndexTrigger;
	bool leftHandTrigger;
	bool rightHandTrigger;

public:
	BonAppetiteApp(rpc::client* connection)
	{
		
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

	PlayerData getPlayerState(ovrTrackingState hmdState) {

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
		output.rightIndexTrigger = rightIndexTrigger;
		output.leftIndexTrigger = leftIndexTrigger;
		output.leftHandTrigger = leftHandTrigger;
		output.rightHandTrigger = rightHandTrigger;

		if (playerNumber == 1) {
			output.headPos = glm::vec3(2.0,0.0,0.0) + output.headPos;
			output.LControlPos = glm::vec3(2.0, 0.0, 0.0) + output.LControlPos;
			output.RControlPos = glm::vec3(2.0, 0.0, 0.0) + output.RControlPos;
		}

		return output;
	}


	void renderScene(const glm::mat4& projection, const glm::mat4& eyePose, ovrEyeRenderDesc eyeRenderDesc) override
	{
		ovrInputState inputState;
		if (OVR_SUCCESS(ovr_GetInputState(_session, ovrControllerType_Touch, &inputState))) {
			if (inputState.IndexTrigger[ovrHand_Right] > 0.5f) {
				rightIndexTrigger = true;
			}
			else {
				rightIndexTrigger = false;
			}

			if (inputState.IndexTrigger[ovrHand_Left] > 0.5f) {
				leftIndexTrigger = true;
			}
			else {
				leftIndexTrigger = false;
			}

			if (inputState.HandTrigger[ovrHand_Left] > 0.5f) {
				leftHandTrigger = true;
			}
			else {
				leftHandTrigger = false;
			}

			if (inputState.HandTrigger[ovrHand_Right] > 0.5f) {
				rightHandTrigger = true;
			}
			else {
				rightHandTrigger = false;
			}
		}

		double ftiming = ovr_GetPredictedDisplayTime(_session, 0);
		ovrTrackingState hmdState = ovr_GetTrackingState(_session,
			ftiming, ovrTrue);
			
		SceneGraph scenegraph = c->call("updatePlayer", getPlayerState(hmdState), playerNumber).as<SceneGraph>();

		scene->update(scenegraph);
	
		scene->render(projection, playerNumber == 0 ? glm::inverse(eyePose) : glm::inverse(glm::translate(glm::mat4(1.0),glm::vec3(2.0,0.0,0.0)) * eyePose), playerNumber, eyeRenderDesc);
		
		
		
	}
};


#endif //BONAPPETITEAPP_H
