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
#include "enums.h"


// a class for building and rendering cubes
class Scene {
  // Program


  //Model* sphere;

  std::vector<Player*> players;
  std::vector<KitchenItem*> tables;
  std::vector<KitchenItem*> appliances;



public:
  Scene() {

	players.push_back(new Player());
	players.push_back(new Player());
	appliances.push_back(new KitchenItem(glm::vec3(0.25, -0.495, -0.75), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0))),glm::vec3(0.01,0.01,0.01)));
	loadTableCoordinates();
  }

  void loadTableCoordinates() {

	  glm::vec3 tableScale(0.25, 0.5, 0.25);
	  tables.push_back(new KitchenItem(glm::vec3(0.25, -1, -0.75),glm::mat4(1.0f),tableScale));//0
	  tables.push_back(new KitchenItem(glm::vec3(0.75, -1, -0.75), glm::mat4(1.0f), tableScale));//1
	  tables.push_back(new KitchenItem(glm::vec3(0.75, -1, -0.25), glm::mat4(1.0f), tableScale));//2
	  tables.push_back(new KitchenItem(glm::vec3(0.75, -1, 0.25), glm::mat4(1.0f), tableScale));//3
	  tables.push_back(new KitchenItem(glm::vec3(0.75, -1, 0.75), glm::mat4(1.0f), tableScale));//4
	  tables.push_back(new KitchenItem(glm::vec3(0.25, -1, 0.75), glm::mat4(1.0f), tableScale));//5
	  tables.push_back(new KitchenItem(glm::vec3(-0.25, -1, 0.75), glm::mat4(1.0f), tableScale));//6
	  tables.push_back(new KitchenItem(glm::vec3(-0.75, -1, 0.75), glm::mat4(1.0f), tableScale));//7
	  tables.push_back(new KitchenItem(glm::vec3(-0.75, -1, 0.25), glm::mat4(1.0f), tableScale));//8
	  tables.push_back(new KitchenItem(glm::vec3(-0.75, -1, -0.25), glm::mat4(1.0f), tableScale));//9
	  tables.push_back(new KitchenItem(glm::vec3(-0.75, -1, -0.75), glm::mat4(1.0f), tableScale));//10
	  tables.push_back(new KitchenItem(glm::vec3(-0.25, -1, -0.75), glm::mat4(1.0f), tableScale));//11
	  tables.push_back(new KitchenItem(glm::vec3(1.25, -1, -0.75), glm::mat4(1.0f), tableScale));//12
	  tables.push_back(new KitchenItem(glm::vec3(1.75, -1, -0.75), glm::mat4(1.0f), tableScale));//13
	  tables.push_back(new KitchenItem(glm::vec3(2.25, -1, -0.75), glm::mat4(1.0f), tableScale));//14
	  tables.push_back(new KitchenItem(glm::vec3(2.25, -1, -0.25), glm::mat4(1.0f), tableScale));//15
	  tables.push_back(new KitchenItem(glm::vec3(2.25, -1, 0.25), glm::mat4(1.0f), tableScale));//16
	  tables.push_back(new KitchenItem(glm::vec3(2.25, -1, 0.75), glm::mat4(1.0f), tableScale));//17
	  tables.push_back(new KitchenItem(glm::vec3(1.75, -1, 0.75), glm::mat4(1.0f), tableScale));//18
	  tables.push_back(new KitchenItem(glm::vec3(1.25, -1, 0.75), glm::mat4(1.0f), tableScale));//19
  }

  void update(PlayerData p, int player) {

	  
		players.at(player)->updateState(p);

		if (!p.rightIndexTrigger && players.at(player)->rightObjectHeld > -1) {
			appliances.at(players.at(player)->rightObjectHeld)->grabbed = false;
			players.at(player)->rightObjectHeld = -1;
		}

		if (!p.leftIndexTrigger && players.at(player)->leftObjectHeld > -1) {
			appliances.at(players.at(player)->leftObjectHeld)->grabbed = false;
			players.at(player)->leftObjectHeld = -1;
		}

		
		for (int i = 0; i < appliances.size(); i++) {

			if (appliances.at(i)->detectCollision(players.at(player)->getTransformedBoundingBox(1))  && !appliances.at(i)->grabbed
				&& players.at(player)->rightObjectHeld == -1 && p.rightIndexTrigger) {
				appliances.at(i)->position = players.at(player)->rightControllerPosition;
				appliances.at(i)->orientation = players.at(player)->rightControllerOrientation;
			};

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
