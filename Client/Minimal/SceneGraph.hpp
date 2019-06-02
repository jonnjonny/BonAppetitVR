#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "ObjectData.hpp"

struct SceneGraph {
	
	PlayerData player1;
	PlayerData player2;
	
	MSGPACK_DEFINE_MAP(player1,player2);

};


#endif //SCENEGRAPH_H
