#ifndef KITCHENITEM_H
#define KITCHENITEM_H

#include "rpc/client.h"

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "objectdata.hpp"
#include "BoundingBox.hpp"

class KitchenItem {
public:
	BoundingBox b;
	glm::vec3 position;
	glm::quat orientation;

	KitchenItem(glm::vec3 pos, glm::quat ori) {
		position = pos;
		orientation = ori;
	}

	bool detectCollision(glm::vec3 cand_position, BoundingBox other) {
		return false;

	}

	void updateItem(ObjectData o) {
		position = o.position;
		orientation = o.orientation;

	}

	ObjectData getState() {
		ObjectData output;
		output.position = position;
		output.orientation = orientation;
		return output;
	}
};


#endif //OBJECTDATA_H

