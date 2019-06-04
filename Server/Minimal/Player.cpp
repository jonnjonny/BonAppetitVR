#include "Player.h"


// Define the coordinates and indices needed to draw the cube. Note that it is not necessary
// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
// This just looks nicer since it's easy to tell what coordinates/indices belong where.


Player::Player() {

  // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
 
}


Player::~Player() {
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
}


void Player::draw( GLuint shaderProgram, const glm::mat4 &projection, const glm::mat4 &view) {
  
}

void Player::updateState(PlayerData p) {
	this->headPose = glm::translate(glm::mat4_cast(p.headOri),p.headPos);
	this->leftControllerOrientation = glm::mat4_cast(p.LControlOri);
	this->leftControllerPosition = glm::translate(glm::mat4(1.0),p.LControlPos);
	this->rightControllerOrientation = glm::mat4_cast(p.RControlOri);
	this->rightControllerPosition = glm::translate(glm::mat4(1.0), p.RControlPos);
}

PlayerData Player::getState() {
	PlayerData output;
	output.headPos = headPose[3];
	output.headOri = glm::mat3(headPose);
	output.LControlPos = glm::vec3(leftControllerPosition * glm::vec4(0.0,0.0,0.0,1.0));
	output.LControlOri = glm::quat_cast(leftControllerOrientation);
	output.RControlPos = glm::vec3(rightControllerPosition * glm::vec4(0.0,0.0,0.0,1.0));
	output.RControlOri = glm::quat_cast(rightControllerOrientation);

	return output;
}
