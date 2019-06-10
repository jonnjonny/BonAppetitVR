#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "ObjectData.hpp"

struct SceneGraph {
	
	PlayerData player1;
	PlayerData player2;
	ObjectData cuttingBoard;
	ObjectData knife;
	ObjectData singleEgg;
	ObjectData standMixer;
	ObjectData barrel;
	ObjectData sugarBowl;
	ObjectData eggCrate;
	ObjectData flourSack;
	ObjectData chocolateSource;
	ObjectData strawberrySource;
	ObjectData crackedEgg;
	ObjectData sugarCube;
	ObjectData flour;
	ObjectData water;
	ObjectData chocolate;
	ObjectData strawberry;
	ObjectData cookieDough;


	MSGPACK_DEFINE_MAP(player1,player2,cuttingBoard,knife,singleEgg,standMixer,barrel,sugarBowl,eggCrate,
					   flourSack, chocolateSource,strawberrySource, crackedEgg, sugarCube, flour, water, chocolate, strawberry,
					   cookieDough);

};


#endif //SCENEGRAPH_H
