//
// Created by Jonathan Leung May 2019.
//

#ifndef BONAPPETITE_SCENE_H
#define BONAPPETITE_SCENE_H


#include <vector>
#include "Core.h"
#include "shader.h"
#include "rpc/client.h"
#include "TexturedCube.h"
#include "Player.h"
#include "SceneGraph.hpp"


// a class for building and rendering cubes
class Scene {
  // Program
  GLuint shaderID;


  //Model* sphere;

  Player* player1;
  Player* player2;


  ////
  //std::vector <glm::mat4> table_positions;


  //std::vector <TexturedCube*> tables;


public:
  Scene() {

    // Shader Program
    shaderID = LoadShaders( "shader.vert", "shader.frag" );

	player1 = new Player();
	player2 = new Player();

    ///
    //populatingTables();

  }



  void render( const glm::mat4 &projection, const glm::mat4 &view, const int playerNumber) {
    ///test purpose only for index 0
	  player1->draw(shaderID, projection, view, playerNumber == 0);
	  player2->draw(shaderID, projection, view, playerNumber == 1);

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



};


#endif //BONAPPETITE_SCENE_H
