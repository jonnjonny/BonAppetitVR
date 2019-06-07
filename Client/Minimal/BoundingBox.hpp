#ifndef BOUNDINGDATA_H
#define BOUNDINGDATA_H

#include "rpc/client.h"
#include "glm/glm.hpp"

struct BoundingBox {

	//float xmin, xmax, ymin, ymax, zmin, zmax;

	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
	glm::vec3 v4;
	glm::vec3 v5;
	glm::vec3 v6;
	glm::vec3 v7;
	glm::vec3 v8;


	MSGPACK_DEFINE_MAP(	v1.x, v1.y, v1.z,
						v2.x, v2.y, v2.z, 
						v3.x, v3.y, v3.z, 
						v4.x, v4.y, v4.z, 
						v5.x, v5.y, v5.z, 
						v6.x, v6.y, v6.z, 
						v7.x, v7.y, v7.z, 
						v8.x, v8.y, v8.z);

};

struct InitialData {

	BoundingBox controller;
	BoundingBox cuttingBoard;
	BoundingBox table;


	MSGPACK_DEFINE_MAP(controller, cuttingBoard, table);

};


#endif //BOUNDINGDATA_H

