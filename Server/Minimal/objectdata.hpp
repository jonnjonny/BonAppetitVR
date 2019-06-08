#ifndef OBJECTDATA_H
#define OBJECTDATA_H

#include "rpc/client.h"
#include "glm/glm.hpp"

struct ObjectData {

	glm::vec3 position;
	glm::quat orientation;
	bool visible;
	float xmin, xmax, ymin, ymax, zmin, zmax;

	MSGPACK_DEFINE_MAP(position.x, position.y, position.z,
		orientation.x, orientation.y, orientation.z, orientation.w, visible,
		xmin,xmax,ymin,ymax,zmin,zmax);

};

struct PlayerData {

	glm::vec3 headPos;
	glm::quat headOri;
	glm::vec3 LControlPos;
	glm::quat LControlOri;
	glm::vec3 RControlPos;
	glm::quat RControlOri;
	bool leftIndexTrigger;
	bool rightIndexTrigger;
	float xmin, xmax, ymin, ymax, zmin, zmax;

	MSGPACK_DEFINE_MAP(headPos.x, headPos.y, headPos.z,
		headOri.x, headOri.y, headOri.z, headOri.w,
		LControlPos.x, LControlPos.y, LControlPos.z,
		LControlOri.x, LControlOri.y, LControlOri.z, LControlOri.w,
		RControlPos.x, RControlPos.y, RControlPos.z,
		RControlOri.x, RControlOri.y, RControlOri.z, RControlOri.w,
		leftIndexTrigger, rightIndexTrigger,
		xmin, xmax, ymin, ymax, zmin, zmax);

};


#endif //OBJECTDATA_H

