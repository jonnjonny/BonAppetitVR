#include "Skybox.h"

#include <GL/glew.h>
#include <iostream>
#include <vector>


Skybox::Skybox(const std::string dir) : TexturedCube(dir)
{
}

Skybox::~Skybox()
{
}

void Skybox::draw(unsigned skyboxShader, const glm::mat4& p, const glm::mat4& v)
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glDepthMask(GL_FALSE);
  TexturedCube::draw(skyboxShader, p, glm::mat4(glm::mat3(v)));
  glDepthMask(GL_TRUE);
  glCullFace(GL_FRONT);
}
