#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "ovr.hpp"

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif


#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cube.h"
#include "objectdata.hpp"
#include "Model.h"


class Player {
public:
  Player();


  ~Player();

  Model* head;
  Model* leftHand;
  Model* rightHand;
  Model* origin;


  glm::mat4 toWorld;
  glm::mat4 headPose;
  glm::mat4 leftControllerPosition;
  glm::mat4 rightControllerPosition;
  glm::mat4 leftControllerOrientation;
  glm::mat4 rightControllerOrientation;


  void draw(GLuint shaderProgram, const glm::mat4 &projection, const glm::mat4 &view, const bool isSelf);

  void Player::updateState(PlayerData p);

};


#endif
