#include "Player.h"


// Define the coordinates and indices needed to draw the cube. Note that it is not necessary
// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
// This just looks nicer since it's easy to tell what coordinates/indices belong where.


Player::Player(glm::vec3 scaleFactor) {

  // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	leftObjectHeld = -1;
	rightObjectHeld = -1;
	scale = scaleFactor;

}


Player::~Player() {
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
}


void Player::draw( GLuint shaderProgram, const glm::mat4 &projection, const glm::mat4 &view) {
  
}

BoundingBox Player::getTransformedBoundingBox(int side) {

	BoundingBox box;
	glm::vec3 controllerPos = (side == 0 ? leftControllerPosition : rightControllerPosition);
	glm::quat controllerOri = (side == 0 ? leftControllerOrientation : rightControllerOrientation);
	
	box.v1 = glm::vec3(glm::translate(glm::mat4(1.0), controllerPos)* glm::mat4_cast(controllerOri)* glm::scale(glm::mat4(1.0),scale) * glm::vec4(b.v1, 1.0));
	box.v2 = glm::vec3(glm::translate(glm::mat4(1.0), controllerPos)* glm::mat4_cast(controllerOri)* glm::scale(glm::mat4(1.0), scale) *glm::vec4(b.v2, 1.0));
	box.v3 = glm::vec3(glm::translate(glm::mat4(1.0), controllerPos)* glm::mat4_cast(controllerOri)* glm::scale(glm::mat4(1.0), scale) *glm::vec4(b.v3, 1.0));
	box.v4 = glm::vec3(glm::translate(glm::mat4(1.0), controllerPos)* glm::mat4_cast(controllerOri)* glm::scale(glm::mat4(1.0), scale) *glm::vec4(b.v4, 1.0));
	box.v5 = glm::vec3(glm::translate(glm::mat4(1.0), controllerPos)* glm::mat4_cast(controllerOri)* glm::scale(glm::mat4(1.0), scale) *glm::vec4(b.v5, 1.0));
	box.v6 = glm::vec3(glm::translate(glm::mat4(1.0), controllerPos)* glm::mat4_cast(controllerOri)* glm::scale(glm::mat4(1.0), scale) *glm::vec4(b.v6, 1.0));
	box.v7 = glm::vec3(glm::translate(glm::mat4(1.0), controllerPos)* glm::mat4_cast(controllerOri)* glm::scale(glm::mat4(1.0), scale) *glm::vec4(b.v7, 1.0));
	box.v8 = glm::vec3(glm::translate(glm::mat4(1.0), controllerPos)* glm::mat4_cast(controllerOri)* glm::scale(glm::mat4(1.0), scale) *glm::vec4(b.v8, 1.0));



	return box;
}

void Player::updateState(PlayerData p) {
	this->headPos = p.headPos;
	this->headOri = p.headOri;
	this->leftControllerOrientation = p.LControlOri;
	this->leftControllerPosition = p.LControlPos;
	this->rightControllerOrientation = p.RControlOri;
	this->rightControllerPosition =  p.RControlPos;
}

PlayerData Player::getState() {
	PlayerData output;
	output.headPos = this->headPos;
	output.headOri = this->headOri;
	output.LControlPos = this->leftControllerPosition;
	output.LControlOri = this->leftControllerOrientation;
	output.RControlPos = this->rightControllerPosition;
	output.RControlOri = this->rightControllerOrientation;

	return output;
}
