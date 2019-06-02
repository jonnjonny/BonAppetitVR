#ifndef WHEREISWALDOVR_EXAMPLEAPP_H
#define WHEREISWALDOVR_EXAMPLEAPP_H


#include "Core.h"
#include "RiftApp.hpp"
#include "Scene.hpp"
#include <GL/glew.h>
#include <ft2build.h>
#include <glm/ext.hpp>

#include FT_FREETYPE_H


struct RenderScenePackage {
	/*glm::mat4 &left_projection;
	glm::mat4 &right_projection;
	ovrPosef &opLeft;
	ovrPosef &opRight;
	ovrEyeType eyeType;*/
	glm::vec3 controllerPosition;
	glm::mat4 left_projection;
	glm::mat4 right_projection;
	glm::mat4 left_headPose;
	glm::mat4 right_headPose;
	ovrEyeType eyeType;
	RenderingOption_X xStatus;
	//SizeOption sizeOption;
	glm::vec3 sizeScalingVector;
};


// An example application that renders a simple cube
class ExampleApp : public RiftApp {
	std::shared_ptr <Scene> scene;


	const float MAX_CUBE_SIZE{ 50.0 };


	const float MIN_CUBE_SIZE{ 1.0 };


	const float INITIAL_CUBE_SIZE{ 30.0 };


	const float RESIZE_STEP{ 0.1f };


	float cubeSize;


	glm::vec3 sizeScalingVector;
	glm::vec3 translatingVector;


public:

	queue <RenderScenePackage> sceneFrames;


	const unsigned int DEFAULT_TRACKING_LAGGING{ 0 };


	const unsigned int DEFAULT_RENDERING_DELAY{ 0 };


	const unsigned int MAX_TRACKING_LAGGING{ 30 };


	const unsigned int MAX_RENDERING_DELAY{ 10 };


	const unsigned int MIN_TRACKING_LAGGING{ 0 };


	const unsigned int MIN_RENDERING_DELAY{ 0 };


	unsigned int trackingLag;


	unsigned int renderingDelay;


	unsigned int currDelayCount;


	RenderScenePackage currentSceneFrame;


	RenderScenePackage poppedSceneFrame;

	std::vector <Line*> lines;

	Line* testLine;


	ExampleApp() {
		trackingLag = DEFAULT_TRACKING_LAGGING;
		renderingDelay = DEFAULT_RENDERING_DELAY;
		currDelayCount = 0;

		std::cout << "Tracking lag: " << trackingLag << " frames" << std::endl;
		std::cout << "Rendering delay : " << renderingDelay << " frames" << std::endl;

	}


protected:

	RenderingOption_X xStatus;


	RenderingOption_A aStatus;


	RenderingOption_B bStatus;


	SizeOption sizeOption;


	int xPressed;


	int aPressed;


	int bPressed;

	int freezed;

	glm::mat4 current_orientation[2];


	glm::mat4 current_position[2];

	int cubeTransfBitMap;

	void initGl() override {
		RiftApp::initGl();
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		ovr_RecenterTrackingOrigin(_session);
		scene = std::shared_ptr <Scene>(new Scene());

		xStatus = RenderingOption_X::ENTIRE_STEREO;
		xPressed = 0;
		aStatus = RenderingOption_A::STEREO;
		aPressed = 0;
		bStatus = RenderingOption_B::REGULAR_TRACKING;
		bPressed = 0;

		//for cube size
		cubeSize = INITIAL_CUBE_SIZE;
		sizeScalingVector = glm::vec3(1.0f);
		translatingVector = glm::vec3(0.0f);

		freezed = 0;
		
		Line* leftScreen4Lines = new Line();
		Line* rightScreen4Lines = new Line();;
		Line* bottomScreen4Lines = new Line();

		lines.push_back(leftScreen4Lines);
		lines.push_back(rightScreen4Lines);
		lines.push_back(bottomScreen4Lines);

		/*
		for (int i = 0; i < lines.size(); i++) {
			for (int j = 0; j < 8; j += 3) {
				if (j % 2 == 1) {
					glm::vec3 vert = glm::vec3(lines[i]->vertices[j],
						lines[i]->vertices[j + 1], lines[i]->vertices[j + 2]);
					vert = glm::vec3(scene->screens[i]->toWorld * glm::vec4(vert, 1.0));
					lines[i]->vertices[j] = vert.x;
					lines[i]->vertices[j + 1] = vert.y;
					lines[i]->vertices[j + 2] = vert.z;
				}
			}
		}*/


		cubeTransfBitMap = 0;

		testLine = new Line();

	}


	void shutdownGl() override {
		scene.reset();
	}


	RenderScenePackage populatePackage(glm::vec3 controllerPosition, glm::mat4 left_projection,
		glm::mat4 right_projection, glm::mat4 left_headPose,
		glm::mat4 right_headPose, ovrEyeType eyeType,
		RenderingOption_X xStatus, glm::vec3 sizeScalingVector) {
		RenderScenePackage sceneInfo;
		sceneInfo.controllerPosition = controllerPosition;
		sceneInfo.left_projection = left_projection;
		sceneInfo.right_projection = right_projection;
		sceneInfo.left_headPose = left_headPose;
		sceneInfo.right_headPose = right_headPose;
		sceneInfo.eyeType = eyeType;
		sceneInfo.xStatus = xStatus;
		sceneInfo.sizeScalingVector = sizeScalingVector;
		return sceneInfo;
	}

	void renderScene(const glm::mat4 &left_projection, const glm::mat4 &right_projection,
		ovrPosef &opLeft, ovrPosef &opRight,
		ovrEyeType eyeType) override {
		

		//RenderScenePackage sceneInfo;

		// get the general state hmdState
		double ftiming = ovr_GetPredictedDisplayTime(_session, 0);
		ovrTrackingState hmdState = ovr_GetTrackingState(_session,
			ftiming, ovrTrue);

		// Get the state of right hand poses
		ovrPoseStatef handPoseStateRight = hmdState.HandPoses[ovrHand_Right];


		// Get Current Input State
		ovrInputState inputState;
		ovr_GetInputState(_session, ovrControllerType_Touch, &inputState);

		checkLeftThumbstick(inputState);

		checkRightThumbstick(inputState);






		//checkLeftIndexTrigger(inputState);

		//checkRightIndexTrigger(inputState);

		//checkLeftIndexTrigger(inputState);

		//checkRightHandTrigger(inputState);





		


		glm::mat4 left_headPose = ovr::toGlm(opLeft);
		glm::mat4 right_headPose = ovr::toGlm(opRight);

		/////
			//store the information of current frame into queue
		RenderScenePackage sceneInfo = populatePackage(ovr::toGlm(handPoseStateRight.ThePose.Position),
			left_projection, right_projection, left_headPose, right_headPose, eyeType, xStatus, sizeScalingVector);

		checkButtonB(inputState, opLeft, opRight);



		

			currentSceneFrame = sceneInfo;

			int doLagAndDelay = 0;
			if (doLagAndDelay) {
				if (inputState.IndexTrigger[ovrHand_Right] > 0.8f) {

					if (trackingLag < MAX_TRACKING_LAGGING) {
						trackingLag++;

					}
					std::cout << "Tracking lag: " << trackingLag << " frames" << std::endl;
				}
				else if (inputState.IndexTrigger[ovrHand_Left] > 0.8f) {
					if (trackingLag > MIN_TRACKING_LAGGING) {
						trackingLag--;
					}
					std::cout << "Tracking lag: " << trackingLag << " frames" << std::endl;
				}

				if (inputState.HandTrigger[ovrHand_Right] > 0.8f) {
					if (renderingDelay < MAX_RENDERING_DELAY) {
						renderingDelay++;

					}
					std::cout << "Rendering delay : " << renderingDelay << " frames" << std::endl;
				}
				else if (inputState.HandTrigger[ovrHand_Left] > 0.8f) {
					if (renderingDelay > MIN_RENDERING_DELAY) {
						renderingDelay--;
					}
					std::cout << "Rendering delay : " << renderingDelay << " frames" << std::endl;
				}
				sceneFrames.push(sceneInfo);

				if (sceneFrames.size() > trackingLag) {
					//currentSceneFrame = sceneFrames.front();
					poppedSceneFrame = sceneFrames.front();
					sceneFrames.pop();
				}


				//      std::cout << "queue has : " << sceneFrames.size() << " elements" << std::endl;


					//only update currentSceneFrame if delay ends, dnm if delay=0
				currDelayCount++;
				if (currDelayCount >= renderingDelay) {
					currDelayCount = 0;
					currentSceneFrame = poppedSceneFrame;
				}
			}
			else { //head in hand mode


				if (inputState.IndexTrigger[ovrHand_Right] > 0.8f) {

					glm::mat4 current_pose_right = ovr::toGlm(handPoseStateRight.ThePose);

					glm::mat4 simulatedLeftEyePos = glm::translate(current_pose_right, glm::vec3(-0.0325f, 0.0f, 0.0f));
					glm::mat4 simulatedRightEyePos = glm::translate(current_pose_right, glm::vec3(0.0325f, 0.0f, 0.0f));

					sceneInfo.left_headPose = simulatedLeftEyePos;
					sceneInfo.right_headPose = simulatedRightEyePos;

					currentSceneFrame = sceneInfo; //directly using the current sceneinfo -> no lag

				}


			}
		


		//display delay and lagging info in terminal



		scene->controllerPosition = currentSceneFrame.controllerPosition;

		//std::cout << currentSceneFrame.sizeScalingVector.x<< ", " << currentSceneFrame.sizeScalingVector.y << ", " << currentSceneFrame.sizeScalingVector.z << std::endl;

		checkButtonX(inputState);

		int disabled[2];

		if (xPressed) {

			std::cout << "sffgdgFR";
			//"random" disable
			if (ovr::toGlm(handPoseStateRight.ThePose.Position).x - ovr::toGlm(handPoseStateRight.ThePose.Position).y
				+ ovr::toGlm(handPoseStateRight.ThePose.Position).z > 0) {
				disabled[0] = 1;
				disabled[1] = 0;
			}
			else {
				disabled[0] = 0;
				disabled[1] = 1;
			}
		}
		else {
			disabled[0] = 0;
			disabled[1] = 0;
		}
		
			if (eyeType == ovrEyeType::ovrEye_Left) {
				//std::cout << inputState.IndexTrigger[ovrHand_Right] << std::endl;

				scene->render(currentSceneFrame.left_projection,
					glm::inverse(currentSceneFrame.left_headPose),
					currentSceneFrame.eyeType,
					disabled[0],
					currentSceneFrame.sizeScalingVector, translatingVector,
					opLeft,
					freezed);
				
			}
			else if (eyeType == ovrEyeType::ovrEye_Right) {
				scene->render(currentSceneFrame.right_projection,
					glm::inverse(currentSceneFrame.right_headPose),
					currentSceneFrame.eyeType,
					disabled[1],
					currentSceneFrame.sizeScalingVector, translatingVector,
					opLeft,
					freezed);
				
			}

			checkButtonA(inputState);
			//if (aPressed) {

				glm::vec3 current_position_right = ovr::toGlm(handPoseStateRight.ThePose.Position);

				glm::vec3 simulatedLeftEyePos = current_position_right;
				simulatedLeftEyePos.x -= 0.0325f;
				glm::vec3 simulatedRightEyePos = current_position_right;
				simulatedRightEyePos.x += 0.0325f;

				//if (eyeType == ovrEyeType::ovrEye_Left)
					//renderLines(lines, currentSceneFrame.eyeType, currentSceneFrame.right_projection, simulatedRightEyePos);
					//renderLines(lines, currentSceneFrame.eyeType, currentSceneFrame.left_projection, simulatedLeftEyePos);


				glm::vec3 pta = glm::vec3(0.0f, -1.2f, -1.697f);

				
				
			//}
		


	}

	glm::mat4 generateTranfMatrix(int cubeTransfBitMap) {
		glm::mat4 transfMat = glm::mat4(1.0f);

		if ((cubeTransfBitMap & (1 << 7)) >> 7) {

		}
		else if ((cubeTransfBitMap & (1 << 6)) >> 6) {

		}
		else if ((cubeTransfBitMap & (1 << 5)) >> 5) {

		}
		else if ((cubeTransfBitMap & (1 << 4)) >> 4) {

		}
		else if ((cubeTransfBitMap & (1 << 3)) >> 3) {

		}
		else if ((cubeTransfBitMap & (1 << 2)) >> 2) {

		}
		else if ((cubeTransfBitMap & (1 << 1)) >> 1) {

		}
		else if ((cubeTransfBitMap & (1)) >> 0 && cubeSize > MIN_CUBE_SIZE) {
			sizeScalingVector = sizeScalingVector / cubeSize;
			cubeSize -= RESIZE_STEP;
			if (cubeSize < MIN_CUBE_SIZE) cubeSize = MIN_CUBE_SIZE;
			sizeScalingVector = cubeSize * sizeScalingVector;
		}
			

			

	

	}

	void checkLeftThumbstick(ovrInputState &inputState) {
		//Thumbstick left
		if (inputState.Thumbstick[ovrHand_Left].x > 0.8f) {
			//std::cout << "expand cube" << std::endl;
			sizeOption = SizeOption::EXPAND;
		}
		else if (inputState.Thumbstick[ovrHand_Left].x < -0.8f) {
			//std::cout << "shrink cube" << std::endl;
			sizeOption = SizeOption::SHRINK;
		}
		else if (inputState.Thumbstick[ovrHand_Left].y < -0.8f) {
			//std::cout << "reset cube" << std::endl;
			sizeOption = SizeOption::RESET;
		}
		else {
			//std::cout << "normal cube" << std::endl;
			sizeOption = SizeOption::ORIGINAL;
		}

		//resize cubes
		if (sizeOption == SizeOption::SHRINK && cubeSize > MIN_CUBE_SIZE) {
			sizeScalingVector = sizeScalingVector / cubeSize;
			cubeSize -= RESIZE_STEP;
			if (cubeSize < MIN_CUBE_SIZE) cubeSize = MIN_CUBE_SIZE;
			sizeScalingVector = cubeSize * sizeScalingVector;
		}
		else if (sizeOption == SizeOption::EXPAND && cubeSize < MAX_CUBE_SIZE) {
			sizeScalingVector = sizeScalingVector / cubeSize;
			cubeSize += RESIZE_STEP;
			if (cubeSize > MAX_CUBE_SIZE) cubeSize = MAX_CUBE_SIZE;
			sizeScalingVector = cubeSize * sizeScalingVector;
		}
		else if (sizeOption == SizeOption::RESET) {
			sizeScalingVector = sizeScalingVector / cubeSize;
			cubeSize = INITIAL_CUBE_SIZE;
			sizeScalingVector = cubeSize * sizeScalingVector;
		}

		if (inputState.Thumbstick[ovrHand_Left].x < -0.8f && cubeSize > MIN_CUBE_SIZE) {
			cubeTransfBitMap = cubeTransfBitMap | (1 << 0);
			sizeScalingVector = sizeScalingVector / cubeSize;
			cubeSize -= RESIZE_STEP;
			if (cubeSize < MIN_CUBE_SIZE) cubeSize = MIN_CUBE_SIZE;
			sizeScalingVector = cubeSize * sizeScalingVector;
		}
		else if (inputState.Thumbstick[ovrHand_Left].x > 0.8f && cubeSize < MAX_CUBE_SIZE) {
			cubeTransfBitMap = cubeTransfBitMap | (1 << 1);
			sizeScalingVector = sizeScalingVector / cubeSize;
			cubeSize += RESIZE_STEP;
			if (cubeSize > MAX_CUBE_SIZE) cubeSize = MAX_CUBE_SIZE;
			sizeScalingVector = cubeSize * sizeScalingVector;
		}
		else if (inputState.Thumbstick[ovrHand_Left].y < -0.8f) {
			cubeTransfBitMap = cubeTransfBitMap | (1 << 2);
			translatingVector.z -= 0.05;

		}
		else if (inputState.Thumbstick[ovrHand_Left].y > 0.8f) {
			cubeTransfBitMap = cubeTransfBitMap | (1 << 3);
			translatingVector.z += 0.05;

		}
		else {
			cubeTransfBitMap = cubeTransfBitMap & (0000);
		}
	}


	void checkRightThumbstick(ovrInputState &inputState) {
		//Thumbstick right
		if (inputState.Thumbstick[ovrHand_Right].x < -0.8f) {
			translatingVector.x -= 0.05;
		}
		else if (inputState.Thumbstick[ovrHand_Right].x > 0.8f) {
			cubeTransfBitMap = cubeTransfBitMap | (1 << 4);
			translatingVector.x += 0.05;

		}
		else if (inputState.Thumbstick[ovrHand_Right].y < -0.8f) {
			cubeTransfBitMap = cubeTransfBitMap | (1 << 6);
			translatingVector.y -= 0.05;

		}
		else if (inputState.Thumbstick[ovrHand_Right].y > 0.8f) {
			cubeTransfBitMap = cubeTransfBitMap | (1 << 7);
			translatingVector.y += 0.05;

		}
		else {
			cubeTransfBitMap = cubeTransfBitMap & (00001111 ) ;
		}
	}


	void checkButtonX(ovrInputState &inputState) {
		//ovrButton_X
		if (inputState.Buttons & ovrButton_X) {

			if (!xPressed) {
				xPressed = 1;
				
			}
		}
		else {
			xPressed = 0;
		} 
	}


	void checkButtonA(ovrInputState &inputState) {
		//ovrButton_A
		if (inputState.Buttons & ovrButton_A) {

			if (!aPressed) {
				aPressed = 1;
			}
		}
		else {
			aPressed = 0;
		}

		
	}


	void checkButtonB(ovrInputState &inputState, ovrPosef &opLeft, ovrPosef &opRight) {

		//ovrButton_B
		if (inputState.Buttons & ovrButton_B) {

			bPressed = 1;
		}
		else {
			bPressed = 0;
		}

		if (bPressed) {
			freezed = 1;
		}
		else {
			freezed = 0;
		}
	}





	void renderLines(std::vector <Line*> lines, ovrEyeType eyeType,
		const glm::mat4 &projection, const glm::mat4 &view) {

		for (int i = 0; i < lines.size(); i++) {/*
			for (int j = 0; j < 8; j+=3) {
				if (j % 2 == 0) {
					lines[i]->vertices[j] = glm::vec3(view * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)).x;
					lines[i]->vertices[j+1] = glm::vec3(view * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)).y;
					lines[i]->vertices[j+2] = glm::vec3(view * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)).z;
				}
				else {
					glm::vec3 vert = glm::vec3(lines[i]->vertices[j],
						lines[i]->vertices[j + 1], lines[i]->vertices[j + 2]);
					vert = glm::vec3( scene->screens[i]->toWorld * glm::vec4(vert,1.0));
					lines[i]->vertices[j] = vert.x ;
					lines[i]->vertices[j+1] = vert.y;
					lines[i]->vertices[j+2] = vert.z;
				}
			}
			
			glBindVertexArray(lines[i]->VAO);
			glBindBuffer(GL_ARRAY_BUFFER, lines[i]->vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(lines[i]->vertices), lines[i]->vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer( 0,3,GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),(GLvoid*)0);
			glBindVertexArray(0);
			lines[i]->draw( scene->lineShader, projection, view);*/
		}
	}	
	
	
	void checkLeftIndexTrigger(ovrInputState &inputState) {
	}


	void checkRightIndexTrigger(ovrInputState &inputState) {
	}


	void checkLeftHandTrigger(ovrInputState &inputState) {
	}


	void checkRightHandTrigger(ovrInputState &inputState) {
	}
};


#endif //WHEREISWALDOVR_EXAMPLEAPP_H
