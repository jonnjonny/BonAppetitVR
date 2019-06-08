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

#include "ObjectData.hpp"
#include "BoundingBox.hpp"


class Player {
public:
  Player();


  ~Player();

  glm::vec3 headPos;
  glm::quat headOri;
  glm::vec3 leftControllerPosition;
  glm::vec3 rightControllerPosition;
  glm::quat leftControllerOrientation;
  glm::quat rightControllerOrientation;
  bool leftHoldingProp;
  bool rightHoldingProp;
  int leftObjectHeld;
  int rightObjectHeld;

  BoundingBox b;

  void draw( GLuint shaderProgram, const glm::mat4 &projection, const glm::mat4 &view);

  void updateState(PlayerData p);

  PlayerData getState();

  BoundingBox getTransformedBoundingBox(int side);

};


#endif
