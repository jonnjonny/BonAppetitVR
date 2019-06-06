#ifndef BOUNDINGDATA_H
#define BOUNDINGDATA_H

#include "rpc/client.h"
#include "glm/glm.hpp"

struct BoundingBox {

	float xmin, xmax, ymin, ymax, zmin, zmax;

	MSGPACK_DEFINE_MAP(xmin, xmax, ymin, ymax, zmin, zmax);

};

struct InitialData {

	BoundingBox controller;
	BoundingBox cuttingBoard;

	MSGPACK_DEFINE_MAP(controller, cuttingBoard);

};


#endif //BOUNDINGDATA_H

