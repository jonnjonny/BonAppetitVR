//
// Created by Jonathan Leung May 2019.
//

#ifndef BONAPPETITE_SCENE_H
#define BONAPPETITE_SCENE_H


#include <vector>
#include "Core.h"
#include "shader.h"
#include "rpc/client.h"
#include "Player.h"
#include "SceneGraph.hpp"
#include "Skybox.h"

#include <random>
#include <stdlib.h>     /* srand, rand */

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"


#include "stb_image.h"

#include <glm/gtx/string_cast.hpp>

#include <vector>
#include "shader.h"


#include "enums.h"




// a class for building and rendering cubes
class Scene {

  //Model* sphere;

  Player* player1;
  Player* player2;


  Skybox* desertbox; //background box


  GLuint shaderID; //shader id for skyboxB
  GLuint woodShaderID;
  GLuint skyBoxShaderID;

  ///controller specific
  Model* sphere;
  glm::vec3 controllerPosition; //controller position passed from exampleApp
  GLuint controllerShaderID;


  ////table specs
  std::vector <glm::mat4> table_positions;
  std::vector <glm::vec3> table_center_positions;
  std::vector <TexturedCube*> tables;


  ///collection of props that constantly updating positions (maybe)
  std::vector<Model*> props;



  GLuint textureShaderID, textureId;
  GLint uniform_texture, attribute_texture;

public:
  Scene() {

    // Shader Program
    shaderID = LoadShaders( "shader.vert", "shader.frag" );
	woodShaderID = LoadShaders("woodShader.vert", "woodShader.frag");
	skyBoxShaderID = LoadShaders("skybox.vert", "skybox.frag");


	//for desert box
	desertbox = new Skybox ("desertbox");
	desertbox->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));

	///controller
	sphere = new Model("./Models/sphere.obj");

	// Shader Program
	controllerShaderID = LoadShaders("shader.vert", "shader.frag");

	populatingTables();


	//woodenBox = new Model("./Models/cube.obj");



	loadingModels();



	glActiveTexture(GL_TEXTURE0);
	textureShaderID = LoadShaders("textureShader.vert", "textureShader.frag");

	//Setting up textures
	unsigned char* data;
	int width, height, numChannels;

	data = stbi_load("./JPG/wood-texture-pattern.jpg", &width, &height, &numChannels, STBI_rgb_alpha);
	if (!data) {
		throw std::runtime_error("Cannot load file waldo.jpg");
	}

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	uniform_texture = glGetUniformLocation(textureShaderID, "tex");
	glUniform1i(uniform_texture, 0);
	glBindTexture(GL_TEXTURE_2D, textureId);



	player1 = new Player();
	player2 = new Player();

    ///
    populatingTables();

  }



  void render( const glm::mat4 &projection, const glm::mat4 &view, const int playerNumber) {

	  desertbox->draw(skyBoxShaderID, projection, view);


	  ///controller
	 // renderController(projection, view, controllerPosition);


	  glDisable(GL_CULL_FACE);

	  //tables
	  render11Tables(projection, view);
/*
	  //rendering props, order matters, add after existing lines!!!!!! Make sure matching the enum class propsID
	  props.at((int)propsID::CHOPPING_BOARD)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[0]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.01, 0.01))* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0));
	  props.at((int)propsID::CHOPPING_BOARD)->Draw(textureShaderID, projection, view, 2);

	  props.at((int)propsID::KNIFE)->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.02, 0)) *glm::translate(glm::mat4(1.0f), table_center_positions[0]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.02, 0.02, 0.02))* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0))* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 0, 1));
	  props.at((int)propsID::KNIFE)->Draw(woodShaderID, projection, view, 2);

	  props.at((int)propsID::SINGLE_EGG)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[1]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.005, 0.005, 0.005))* glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	  props.at((int)propsID::SINGLE_EGG)->Draw(textureShaderID, projection, view, 2);

	  props.at((int)propsID::STAND_MIXER)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[2]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.01, 0.01))* glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	  props.at((int)propsID::STAND_MIXER)->Draw(textureShaderID, projection, view, 2);

	  props.at((int)propsID::BARREL)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[11]) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0));
	  props.at((int)propsID::BARREL)->Draw(textureShaderID, projection, view, 2);

	  props.at((int)propsID::SUGAR_BOWL)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[4]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05))* glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0, 1, 0));
	  props.at((int)propsID::SUGAR_BOWL)->Draw(textureShaderID, projection, view, 2);
 */
	  player1->draw(shaderID, projection, view, playerNumber == 0);
	  player2->draw(shaderID, projection, view, playerNumber == 1);
	 
  }



  void loadingModels() {
	  props.push_back(new Model("./Models/ChoppingBoard.obj"));
	  props.push_back(new Model("./Models/Knife.obj"));
	  props.push_back(new Model("./Models/SingleEgg.obj"));
	  props.push_back(new Model("./Models/StandMixer.obj"));
	  props.push_back(new Model("./Models/teapot_s0.obj"));
	  props.push_back(new Model("./Models/SugarBowl.obj"));

  }



  void render11Tables(const glm::mat4 &projection, const glm::mat4 &view) {
	  for (int i = 0; i < tables.size(); ++i) {
		  tables[i]->toWorld = table_positions[i];
		  tables[i]->draw(skyBoxShaderID, projection, view);
	  }
  }

  void populatingTables() {
	  for (int i = 0; i < 12; ++i) {
		  tables.push_back(new TexturedCube("Martini"));
	  }

	  glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.25, 0.5, 0.25));//glm::vec3(0.25, 0.5, 0.25));
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.25, -1, -0.75)) * scaleMatrix);//0
	  //std::cout << glm::to_string(table_positions.at(table_positions.size() - 1)) << std::endl;
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.75, -1, -0.75)) * scaleMatrix);//1
	  //std::cout << glm::to_string(glm::vec4(0.5, 0, 0,1) * table_positions.at(0)   ) << std::endl;

	  //std::cout << glm::to_string(table_positions.at(1)/table_positions.at(0)) << std::endl;

	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.75, -1, -0.25)) * scaleMatrix);//2
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.75, -1, 0.25)) * scaleMatrix);//3
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.75, -1, 0.75)) * scaleMatrix);//4
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.25, -1, 0.75)) * scaleMatrix);//5
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-0.25, -1, 0.75)) * scaleMatrix);//6
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-0.75, -1, 0.75)) * scaleMatrix);//7
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-0.75, -1, 0.25)) * scaleMatrix);//8
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-0.75, -1, -0.25)) * scaleMatrix);//9
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-0.75, -1, -0.75)) * scaleMatrix);//10
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-0.25, -1, -0.75)) * scaleMatrix);//11
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(1.25, -1, -0.75)) * scaleMatrix);//12
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(1.75, -1, -0.75)) * scaleMatrix);//13
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(2.25, -1, -0.75)) * scaleMatrix);//14
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(2.25, -1, -0.25)) * scaleMatrix);//15
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(2.25, -1, 0.25)) * scaleMatrix);//16
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(2.25, -1, 0.75)) * scaleMatrix);//17
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(1.75, -1, 0.75)) * scaleMatrix);//18
	  table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(1.25, -1, 0.75)) * scaleMatrix);//19


	  table_center_positions.push_back(glm::vec3(0.25, -0.495, -0.75));//0
	  table_center_positions.push_back(glm::vec3(0.75, -0.495, -0.75));//1
	  table_center_positions.push_back(glm::vec3(0.75, -0.495, -0.25));//2
	  table_center_positions.push_back(glm::vec3(0.75, -0.495, 0.25));//3
	  table_center_positions.push_back(glm::vec3(0.75, -0.495, 0.75));//4 
	  table_center_positions.push_back(glm::vec3(0.25, -0.495, 0.75));//5
	  table_center_positions.push_back(glm::vec3(-0.25, -0.495, 0.75));//6
	  table_center_positions.push_back(glm::vec3(-0.75, -0.495, 0.75));//7
	  table_center_positions.push_back(glm::vec3(-0.75, -0.495, 0.25)); //8
	  table_center_positions.push_back(glm::vec3(-0.75, -0.495, -0.25));//9
	  table_center_positions.push_back(glm::vec3(-0.75, -0.495, -0.75));//10
	  table_center_positions.push_back(glm::vec3(-0.25, -0.495, -0.75));//11

  }

  void updatePlayer(SceneGraph s) {

		  player1->updateState(s.player1);
		  player2->updateState(s.player2);

  }



  void renderController(const glm::mat4 &projection, const glm::mat4 &view,
	  const glm::vec3 &controllerPosition) {
	  sphere->toWorld =
		  glm::translate(glm::mat4(1.0f), controllerPosition) *
		  glm::scale(glm::mat4(1.0f), vec3(0.0175, 0.0175, 0.0175));
	 // sphere->Draw(controllerShaderID, projection, view, 2);
  }


};


#endif //BONAPPETITE_SCENE_H
