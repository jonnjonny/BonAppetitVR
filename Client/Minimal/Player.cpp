#include "Player.h"


// Define the coordinates and indices needed to draw the cube. Note that it is not necessary
// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
// This just looks nicer since it's easy to tell what coordinates/indices belong where.


Player::Player() {
	//head = new Model("pigavatarchef.obj");
	//leftHand = new Model("pigavatarchef.obj");
	//rightHand = new Model("pigavatarchef.obj");

	head = new Cube();
	leftHand = new Cube();
	rightHand = new Cube();
	headColorMap = { {0,1},{1,1},{2, 1},{3,1}, { 4,0},{5,0},{7,0},{8 , 0},{6,1},{9,2} };
}


Player::~Player() {
	delete head;
	delete leftHand;
	delete rightHand;
}


void Player::draw(GLuint shaderProgram, const glm::mat4 &projection, const glm::mat4 &view, const bool isSelf) {

	leftHand->toWorld = leftControllerPosition * leftControllerOrientation * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0)) * glm::scale(glm::mat4(1.0),glm::vec3(0.01,0.01,0.01));
	rightHand->toWorld = rightControllerPosition * rightControllerOrientation * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0)) * glm::scale(glm::mat4(1.0), glm::vec3(0.01, 0.01, 0.01));

	if (!isSelf) {
		head->toWorld = headPosition * headOrientation * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0)) * glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.1, 0.1));
		head->draw(shaderProgram, projection, view);
	}

	leftHand->draw(shaderProgram, projection, view);
	rightHand->draw(shaderProgram, projection, view);
	
}

void Player::updateState(PlayerData p) {
	this->headPosition = glm::translate(glm::mat4(1.0),p.headPos);
	this->headOrientation = glm::mat4_cast(p.headOri);
	this->leftControllerOrientation = glm::mat4_cast(p.LControlOri);
	this->leftControllerPosition = glm::translate(glm::mat4(1.0), p.LControlPos);
	this->rightControllerOrientation = glm::mat4_cast(p.RControlOri);
	this->rightControllerPosition = glm::translate(glm::mat4(1.0), p.RControlPos);
}
