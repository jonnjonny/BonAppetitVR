//
// Created by Jonathan Leung May 2019.
//

#ifndef BONAPPETITE_SCENE_H
#define BONAPPETITE_SCENE_H


#include <vector>
#include <unordered_map>
#include "Core.h"
#include "Player.h"
#include "rpc/server.h"
#include "BoundingBox.hpp"
#include "KitchenItem.hpp"
#include "SceneGraph.hpp"
#include "../../Client/Minimal/enums.h"


// a class for building and rendering cubes
class Scene {
  // Program


  //Model* sphere;

  std::vector<Player*> players;
  std::vector<KitchenItem*> tables;
  std::vector<KitchenItem*> appliances;
  std::vector<KitchenItem*> ingredients;
  std::unordered_map<int, int> propToIngredient;
  int eggBasketLocation, eggCrackCount;



public:
  Scene() {

	players.push_back(new Player(glm::vec3(0.1,0.1,0.1)));
	players.push_back(new Player(glm::vec3(0.1,0.1,0.1)));
	appliances.push_back(new KitchenItem(glm::vec3(0.25, -0.495, -0.75), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0))),glm::vec3(0.01,0.01,0.01))); //CHOPPING BOARD
	appliances.push_back(new KitchenItem(glm::vec3(0.25, -0.475, -0.75), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0))), glm::vec3(0.02, 0.02, 0.02))); //KNIFE
	appliances.push_back(new KitchenItem(glm::vec3(0.75, -0.495, -0.25), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0))), glm::vec3(0.01, 0.01, 0.01))); //STAND MIXER
	appliances.push_back(new KitchenItem(glm::vec3(0.75, -0.495, 0.25), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0))), glm::vec3(1, 1, 1))); //BARREL
	appliances.push_back(new KitchenItem(glm::vec3(0.75, -0.495, 0.75), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0, 1, 0))), glm::vec3(0.5, 0.5, 0.5))); //SUGAR BOWL
	appliances.push_back(new KitchenItem(glm::vec3(-0.75, -0.495, -0.75), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f),glm::vec3(0,1,0))), glm::vec3(0.05, 0.05, 0.05))); //EGG CRATE
	appliances.push_back(new KitchenItem(glm::vec3(-0.75, -0.495, 0.75), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0))), glm::vec3(0.005, 0.005, 0.005))); //FLOUR SACK
	//appliances.push_back(new KitchenItem(glm::vec3(-0.75, -0.495, -0.25), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0))), glm::vec3(0.005, 0.005, 0.005))); //CHOCOLATE SOURCE
	//appliances.push_back(new KitchenItem(glm::vec3(-0.75, -0.495, 0.25), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0))), glm::vec3(0.005, 0.005, 0.005))); //STRAWBERRY SOURCE
	
	//Ingredients

	ingredients.push_back(new KitchenItem(glm::vec3(-15.0, -0.495, -0.75), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0))), glm::vec3(0.001, 0.001, 0.001),false)); //SINGLE EGG
	ingredients.push_back(new KitchenItem(glm::vec3(-15.0, -0.495, -0.75), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0))), glm::vec3(0.5, 0.5, 0.5), false)); //CRACKED EGG
	ingredients.push_back(new KitchenItem(glm::vec3(-15.0, -0.495, -0.75), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0))), glm::vec3(0.5, 0.5, 0.5), false)); //SUGAR CUBE
	ingredients.push_back(new KitchenItem(glm::vec3(-15.0, -0.495, -0.75), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0))), glm::vec3(0.05, 0.05, 0.05), false)); //FLOUR
	//ingredients.push_back(new KitchenItem(glm::vec3(-15.0, -0.495, -0.75), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0))), glm::vec3(0.05, 0.05, 0.05), false)); //WATER
	//ingredients.push_back(new KitchenItem(glm::vec3(-15.0, -0.495, -0.75), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0))), glm::vec3(0.05, 0.05, 0.05), false)); //CHOCOLATE
	//ingredients.push_back(new KitchenItem(glm::vec3(-15.0, -0.495, -0.75), glm::quat(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0))), glm::vec3(0.05, 0.05, 0.05), false)); //STRAWBERRY
	loadTableCoordinates();

	propToIngredient = { {(int)propsID::EGG_CRATE,(int)ingredientsID::SINGLE_EGG}, {(int)propsID::SUGAR_BOWL,(int)ingredientsID::SUGAR_CUBE},
	{(int)propsID::FLOUR_SACK,(int)ingredientsID::FLOUR}, {(int)propsID::CHOCOLATE,(int)ingredientsID::CHOCOLATE}, {(int)propsID::STRAWBERRY,(int)ingredientsID::STRAWBERRY},
	{(int)propsID::BARREL,(int)ingredientsID::WATER} };

	//Setting preset rotations
	appliances.at((int)propsID::KNIFE)->toWorld = glm::rotate(glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(1, 0, 0)), glm::radians(90.0f), glm::vec3(0, 1, 0));
	ingredients.at((int)ingredientsID::SINGLE_EGG)->toWorld = glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(1, 0, 0));
	ingredients.at((int)ingredientsID::FLOUR)->toWorld = glm::rotate(glm::mat4(1.0),glm::radians(180.0f), glm::vec3(0,0,1));
  
	eggBasketLocation = 10;
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

		//If right hand trigger is pressed, see if there are any appliances collided that will give you an ingredient
		if (!p.rightIndexTrigger && p.rightHandTrigger) {
			BoundingBox playerBox = players.at(player)->getTransformedBoundingBox(1);
			if (players.at(player)->rightObjectHeld == -1) {
				for (int i = 0; i < appliances.size(); i++) {
					if (propToIngredient.find(i) != propToIngredient.end() && appliances.at(i)->detectCollision(playerBox)) {
						ingredients.at(propToIngredient.at(i))->position = players.at(player)->rightControllerPosition;
						ingredients.at(propToIngredient.at(i))->orientation = glm::quat(glm::mat4_cast(players.at(player)->rightControllerOrientation) * ingredients.at(propToIngredient.at(i))->toWorld);
						ingredients.at(propToIngredient.at(i))->isVisible = true;
						players.at(player)->rightObjectHeld = propToIngredient.at(i);
						players.at(player)->rightHoldingProp = false;
						ingredients.at(propToIngredient.at(i))->grabbed = true;
						break;

					}
				}
			}
			else {

				//Keep ingredient in hand if still pressed
				if (!players.at(player)->rightHoldingProp && ingredients.at(players.at(player)->rightObjectHeld)->detectCollision(playerBox)) {
					ingredients.at(players.at(player)->rightObjectHeld)->position = players.at(player)->rightControllerPosition;
					ingredients.at(players.at(player)->rightObjectHeld)->orientation = glm::quat(glm::mat4_cast(players.at(player)->rightControllerOrientation) * ingredients.at(players.at(player)->rightObjectHeld)->toWorld);
					ingredients.at(players.at(player)->rightObjectHeld)->isVisible = true;
					players.at(player)->rightHoldingProp = false;
					ingredients.at(players.at(player)->rightObjectHeld)->grabbed = true;
				}

				//If egg collides with table, then change egg to cracked state
				if (!players.at(player)->rightHoldingProp && players.at(player)->rightObjectHeld == 0) {
					BoundingBox rightIngredientBox = ingredients.at(0)->getTransformedBoundingBox();
					for (int i = 0; i < tables.size(); i++) {
						if (eggBasketLocation != i && tables.at(i)->detectCollision(rightIngredientBox)) {
							ingredients.at(0)->position = ingredients.at(0)->originalPosition;
							ingredients.at(0)->orientation = ingredients.at(0)->originalOrientation;
							ingredients.at(0)->isVisible = false;
							ingredients.at(0)->grabbed = false;
							ingredients.at(1)->position = players.at(player)->rightControllerPosition;
							ingredients.at(1)->orientation = players.at(player)->rightControllerOrientation;
							ingredients.at(1)->isVisible = true;
							ingredients.at(1)->grabbed = true;
							players.at(player)->rightHoldingProp = false;
							players.at(player)->rightObjectHeld = 1;
							break;
						}
					}
				}

			}
		}


		//If left index trigger released and appliance is still in hand and is colliding to table, snap appliance onto table
		if (!p.leftIndexTrigger && players.at(player)->leftHoldingProp && players.at(player)->leftObjectHeld > -1) {
			BoundingBox leftApplianceBox = appliances.at(players.at(player)->leftObjectHeld)->getTransformedBoundingBox();
			for (int i = 0; i < tables.size(); i++) {
				if (tables.at(i)->detectCollision(leftApplianceBox)) {
					glm::vec3 snappedPosition = tables.at(i)->position;
					snappedPosition.y = appliances.at(players.at(player)->leftObjectHeld)->originalPosition.y;
					appliances.at(players.at(player)->leftObjectHeld)->position = snappedPosition;
					appliances.at(players.at(player)->leftObjectHeld)->orientation = appliances.at(players.at(player)->leftObjectHeld)->originalOrientation;
					if (players.at(player)->leftObjectHeld == (int)propsID::EGG_CRATE) eggBasketLocation = i;
					break;
				}
			}
			appliances.at(players.at(player)->leftObjectHeld)->grabbed = false;
			players.at(player)->leftObjectHeld = -1;
		}


		//If right index trigger released and appliance is still in hand and is colliding to table, snap appliance onto table
		if (!p.rightIndexTrigger && players.at(player)->rightHoldingProp && players.at(player)->rightObjectHeld > -1) {
			BoundingBox rightApplianceBox = appliances.at(players.at(player)->rightObjectHeld)->getTransformedBoundingBox();

			for (int i = 0; i < tables.size(); i++) {
				if (tables.at(i)->detectCollision(rightApplianceBox)) {
					glm::vec3 snappedPosition = tables.at(i)->position;
					snappedPosition.y = appliances.at(players.at(player)->rightObjectHeld)->originalPosition.y;
					appliances.at(players.at(player)->rightObjectHeld)->position = snappedPosition;
					appliances.at(players.at(player)->rightObjectHeld)->orientation = appliances.at(players.at(player)->rightObjectHeld)->originalOrientation;
					if (players.at(player)->rightObjectHeld == (int)propsID::EGG_CRATE) eggBasketLocation = i;
					break;
				}
			}
			appliances.at(players.at(player)->rightObjectHeld)->grabbed = false;
			players.at(player)->rightObjectHeld = -1;
		}

		//If right hand trigger released, ingredient should disappear
		if (!p.rightHandTrigger && !players.at(player)->rightHoldingProp && players.at(player)->rightObjectHeld > -1) {
			ingredients.at(players.at(player)->rightObjectHeld)->grabbed = false;
			ingredients.at(players.at(player)->rightObjectHeld)->isVisible= false;
			ingredients.at(players.at(player)->rightObjectHeld)->position = ingredients.at(players.at(player)->rightObjectHeld)->originalPosition;
			ingredients.at(players.at(player)->rightObjectHeld)->orientation = ingredients.at(players.at(player)->rightObjectHeld)->originalOrientation;
			players.at(player)->rightObjectHeld = -1;
		}

		//If index trigger in either left or right hand is pressed, then if controller collides with appliance, move appliance with controller
		for (int i = 0; i < appliances.size(); i++) {

			if (appliances.at(i)->detectCollision(players.at(player)->getTransformedBoundingBox(0))
				&& p.leftIndexTrigger && !p.leftHandTrigger && ((players.at(player)->leftObjectHeld == -1 && !appliances.at(i)->grabbed) || (players.at(player)->leftHoldingProp && players.at(player)->leftObjectHeld == i))) {
				appliances.at(i)->position = players.at(player)->leftControllerPosition;
				appliances.at(i)->orientation = glm::quat(glm::mat4_cast(players.at(player)->leftControllerOrientation)* appliances.at(i)->toWorld);
				players.at(player)->leftObjectHeld = i;
				players.at(player)->leftHoldingProp = true;
				appliances.at(i)->grabbed = true;
				break;
			}
		}

		for (int i = 0; i < appliances.size(); i++) {

			if (appliances.at(i)->detectCollision(players.at(player)->getTransformedBoundingBox(1))
				&& p.rightIndexTrigger && !p.rightHandTrigger && ((players.at(player)->rightObjectHeld == -1 && !appliances.at(i)->grabbed) || (players.at(player)->rightHoldingProp && players.at(player)->rightObjectHeld == i))) {
				appliances.at(i)->position = players.at(player)->rightControllerPosition;
				appliances.at(i)->orientation = glm::quat(glm::mat4_cast(players.at(player)->rightControllerOrientation)* appliances.at(i)->toWorld );
				players.at(player)->rightObjectHeld = i;
				players.at(player)->rightHoldingProp = true;
				appliances.at(i)->grabbed = true;
				break;
			}

		}

  }

  void loadInitialData (InitialData b) {
	  players.at(0)->b = b.controller;
	  players.at(1)->b = b.controller;
	  appliances.at((int)propsID::CHOPPING_BOARD)->objectSpaceBoundingBox = b.cuttingBoard;
	  appliances.at((int)propsID::KNIFE)->objectSpaceBoundingBox = b.knife;
	  
	  appliances.at((int)propsID::STAND_MIXER)->objectSpaceBoundingBox = b.standMixer;
	  appliances.at((int)propsID::BARREL)->objectSpaceBoundingBox = b.barrel;
	  appliances.at((int)propsID::SUGAR_BOWL)->objectSpaceBoundingBox = b.sugarBowl;
	  appliances.at((int)propsID::EGG_CRATE)->objectSpaceBoundingBox = b.eggCrate;
	  appliances.at((int)propsID::FLOUR_SACK)->objectSpaceBoundingBox = b.flourSack;
	  //appliances.at((int)propsID::CHOCOLATE)->objectSpaceBoundingBox = b.chocolate;
	  //appliances.at((int)propsID::STRAWBERRY)->objectSpaceBoundingBox = b.strawberry;

	  ingredients.at((int)ingredientsID::SINGLE_EGG)->objectSpaceBoundingBox = b.singleEgg;
	  ingredients.at((int)ingredientsID::CRACKED_EGG)->objectSpaceBoundingBox = b.crackedEgg;
	  ingredients.at((int)ingredientsID::SUGAR_CUBE)->objectSpaceBoundingBox = b.sugarCube;
	  ingredients.at((int)ingredientsID::FLOUR)->objectSpaceBoundingBox = b.flour;
	  //ingredients.at((int)ingredientsID::WATER)->objectSpaceBoundingBox = b.water;
	  //ingredients.at((int)ingredientsID::CHOCOLATE)->objectSpaceBoundingBox = b.chocolate;
	  //ingredients.at((int)ingredientsID::STRAWBERRY)->objectSpaceBoundingBox = b.strawberry;

	  for(int i = 0; i < tables.size(); i++) tables.at(i)->objectSpaceBoundingBox = b.table;
  }

  SceneGraph Scene::getGraph() {
	  SceneGraph output;
	  output.player1 = players.at(0)->getState();
	  output.player2 = players.at(1)->getState();
	  output.cuttingBoard = appliances.at((int)propsID::CHOPPING_BOARD)->getState();
	  output.knife = appliances.at((int)propsID::KNIFE)->getState();
	  output.standMixer = appliances.at((int)propsID::STAND_MIXER)->getState();
	  output.barrel = appliances.at((int)propsID::BARREL)->getState();
	  output.sugarBowl = appliances.at((int)propsID::SUGAR_BOWL)->getState();
	  output.eggCrate = appliances.at((int)propsID::EGG_CRATE)->getState();
	  output.flourSack = appliances.at((int)propsID::FLOUR_SACK)->getState();
	  //output.chocolateSource = appliances.at((int)propsID::CHOCOLATE)->getState();
	  //output.strawberrySource = appliances.at((int)propsID::STRAWBERRY)->getState();

	  output.singleEgg = ingredients.at((int)ingredientsID::SINGLE_EGG)->getState();
	  output.crackedEgg = ingredients.at((int)ingredientsID::CRACKED_EGG)->getState();
	  output.sugarCube = ingredients.at((int)ingredientsID::SUGAR_CUBE)->getState();
	  output.flour = ingredients.at((int)ingredientsID::FLOUR)->getState();
	  //output.water = ingredients.at((int)ingredientsID::WATER)->getState();
	  //output.chocolate = ingredients.at((int)ingredientsID::CHOCOLATE)->getState();
	  //output.strawberry = ingredients.at((int)ingredientsID::STRAWBERRY)->getState();
	  return output;

  }



};


#endif //BONAPPETITE_SCENE_H
