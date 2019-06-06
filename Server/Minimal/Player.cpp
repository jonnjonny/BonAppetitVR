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
