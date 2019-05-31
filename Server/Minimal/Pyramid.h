#ifndef PYRAMID_H
#define PYRAMID_H

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

#include <vector>
#include <iostream>


class Pyramid{
public:
  Pyramid(std::vector<float> vertex);
  ~Pyramid();

  glm::mat4 toWorld;
  float vertices[15];

  void draw(GLuint shaderProgram, const glm::mat4& projection, const glm::mat4& view, glm::vec3 eyePos, int color);

  // These variables are needed for the shader program
  GLuint VAO,VBO,EBO;
  GLuint uProjection, uModelview, uLinecolor;
};

#endif
