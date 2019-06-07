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


	MSGPACK_DEFINE_MAP(player1,player2,cuttingBoard,knife,singleEgg,standMixer,barrel,sugarBowl);

};


#endif //SCENEGRAPH_H
