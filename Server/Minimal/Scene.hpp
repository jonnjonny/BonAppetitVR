//
// Created by Jonathan Leung May 2019.
//

#ifndef BONAPPETITE_SCENE_H
#define BONAPPETITE_SCENE_H


#include <vector>
#include "Core.h"
#include "Player.h"
#include "rpc/server.h"
#include "BoundingBox.hpp"
#include "KitchenItem.hpp"
#include "SceneGraph.hpp"


// a class for building and rendering cubes
class Scene {
  // Program


  //Model* sphere;

  Player* player1;
  Player* player2;
  std::vector<KitchenItem*> tables;
  KitchenItem* cuttingBoard;


public:
  Scene() {

	player1 = new Player();
	player2 = new Player();
	cuttingBoard = new KitchenItem(glm::vec3(0.25, -0.495, -0.75), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0))),0.01f);
	loadTableCoordinates();
  }

  void loadTableCoordinates() {
	  tables.push_back(new KitchenItem(glm::vec3(0.25, -1, -0.75),glm::mat4(1.0f),);//0
	  tables.push_back(glm::vec3(0.75, -1, -0.75));//1
	  tables.push_back(glm::vec3(0.75, -1, -0.25));//2
	  tables.push_back(glm::vec3(0.75, -1, 0.25));//3
	  tables.push_back(glm::vec3(0.75, -1, 0.75));//4
	  tables.push_back(glm::vec3(0.25, -1, 0.75));//5
	  tables.push_back(glm::vec3(-0.25, -1, 0.75));//6
	  tables.push_back(glm::vec3(-0.75, -1, 0.75));//7
	  tables.push_back(glm::vec3(-0.75, -1, 0.25));//8
	  tables.push_back(glm::vec3(-0.75, -1, -0.25));//9
	  tables.push_back(glm::vec3(-0.75, -1, -0.75));//10
	  tables.push_back(glm::vec3(-0.25, -1, -0.75));//11
	  tables.push_back(glm::vec3(1.25, -1, -0.75));//12
	  tables.push_back(glm::vec3(1.75, -1, -0.75));//13
	  tables.push_back(glm::vec3(2.25, -1, -0.75));//14
	  tables.push_back(glm::vec3(2.25, -1, -0.25));//15
	  tables.push_back(glm::vec3(2.25, -1, 0.25));//16
	  tables.push_back(glm::vec3(2.25, -1, 0.75));//17
	  tables.push_back(glm::vec3(1.75, -1, 0.75));//18
	  tables.push_back(glm::vec3(1.25, -1, 0.75));//19
  }

  void update(PlayerData p, int player) {

	  if (player == 0) {
		  player1->updateState(p);
		  if (cuttingBoard->detectCollision(player1->getTransformedBoundingBox()) && p.rightIndexTrigger) {
			  std::cout << "Collision Detected" << std::endl;
			  cuttingBoard->position = player1->rightControllerPosition;
			  cuttingBoard->orientation = player1->rightControllerOrientation;
		  };
	  }
	  else {
		  player2->updateState(p);
	  }

  }

  void loadInitialData (InitialData b) {
	  player1->b = b.controller;
	  player2->b = b.controller;
	  cuttingBoard->objectSpaceBoundingBox = b.cuttingBoard;
  }

  SceneGraph Scene::getGraph() {
	  SceneGraph output;
	  output.player1 = player1->getState();
	  output.player2 = player2->getState();
	  output.cuttingBoard = cuttingBoard->getState();
	  return output;

  }



};


#endif //BONAPPETITE_SCENE_H
