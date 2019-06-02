//
// Created by Jonathan Leung May 2019.
//

#ifndef BONAPPETITE_SCENE_H
#define BONAPPETITE_SCENE_H


#include <vector>
#include "Core.h"
#include "Player.h"
#include "rpc/server.h"
#include "SceneGraph.hpp"


// a class for building and rendering cubes
class Scene {
  // Program


  //Model* sphere;

  Player* player1;
  Player* player2;


public:
  Scene() {

	player1 = new Player();
	player2 = new Player();

  }

  void updatePlayer(PlayerData p, int player) {

	  if (player == 0) {
		  player1->updateState(p);
	  }
	  else {
		  player2->updateState(p);
	  }

  }

  SceneGraph Scene::getGraph() {
	  SceneGraph output;
	  output.player1 = player1->getState();
	  output.player2 = player2->getState();

	  return output;

  }



};


#endif //BONAPPETITE_SCENE_H
