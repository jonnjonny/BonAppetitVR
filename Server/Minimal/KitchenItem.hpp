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
		float xmin1 = b.xmin + position.x;
		float xmax1 = b.xmax + position.x;
		float ymin1 = b.ymin + position.y;
		float ymax1 = b.ymax + position.y;
		float zmin1 = b.zmin + position.z;
		float zmax1 = b.zmax + position.z;

		float xmin2 = other.xmin + cand_position.x;
		float xmax2 = other.xmax + cand_position.x;
		float ymin2 = other.ymin + cand_position.y;
		float ymax2 = other.ymax + cand_position.y;
		float zmin2 = other.zmin + cand_position.z;
		float zmax2 = other.zmax + cand_position.z;

		return !((xmin1 <= xmax2 || xmin1 <= xmax1) &&
			(ymin1 <= ymax2 || ymin1 <= ymax1) &&
			(zmin1 <= zmax2 || zmin1 <= zmax1));

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

