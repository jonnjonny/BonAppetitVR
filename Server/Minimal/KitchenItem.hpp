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
#include <limits>

class KitchenItem {
public:
	BoundingBox objectSpaceBoundingBox;
	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;

	KitchenItem(glm::vec3 pos, glm::quat ori, glm::vec3 scaleFactor) {
		position = pos;
		orientation = ori;
		scale = scaleFactor;
	}

	BoundingBox getTransformedBoundingBox() {

		BoundingBox box;

		box.v1 = glm::vec3(glm::translate(glm::mat4(1.0), position)* glm::mat4_cast(orientation)*glm::scale(glm::mat4(1.0), scale) * glm::vec4(objectSpaceBoundingBox.v1, 1.0));
		box.v2 = glm::vec3(glm::translate(glm::mat4(1.0), position)* glm::mat4_cast(orientation)*glm::scale(glm::mat4(1.0), scale) * glm::vec4(objectSpaceBoundingBox.v2, 1.0));
		box.v3 = glm::vec3(glm::translate(glm::mat4(1.0), position)* glm::mat4_cast(orientation)*glm::scale(glm::mat4(1.0), scale) * glm::vec4(objectSpaceBoundingBox.v3, 1.0));
		box.v4 = glm::vec3(glm::translate(glm::mat4(1.0), position)* glm::mat4_cast(orientation)*glm::scale(glm::mat4(1.0), scale) * glm::vec4(objectSpaceBoundingBox.v4, 1.0));
		box.v5 = glm::vec3(glm::translate(glm::mat4(1.0), position)* glm::mat4_cast(orientation)*glm::scale(glm::mat4(1.0), scale) * glm::vec4(objectSpaceBoundingBox.v5, 1.0));
		box.v6 = glm::vec3(glm::translate(glm::mat4(1.0), position)* glm::mat4_cast(orientation)*glm::scale(glm::mat4(1.0), scale) * glm::vec4(objectSpaceBoundingBox.v6, 1.0));
		box.v7 = glm::vec3(glm::translate(glm::mat4(1.0), position)* glm::mat4_cast(orientation)*glm::scale(glm::mat4(1.0), scale) * glm::vec4(objectSpaceBoundingBox.v7, 1.0));
		box.v8 = glm::vec3(glm::translate(glm::mat4(1.0), position)* glm::mat4_cast(orientation)*glm::scale(glm::mat4(1.0), scale) * glm::vec4(objectSpaceBoundingBox.v8, 1.0));
		
		return box;
	}

	std::vector<glm::vec3> convertBoundingBoxToVector(BoundingBox b) {
		std::vector<glm::vec3> output;

		output.push_back(b.v1);
		output.push_back(b.v2);
		output.push_back(b.v3);
		output.push_back(b.v4);
		output.push_back(b.v5);
		output.push_back(b.v6);
		output.push_back(b.v7);
		output.push_back(b.v8);

		return output;
	}

	bool checkAxisProjectionOverlap(std::vector<glm::vec3> a, std::vector<glm::vec3> b, glm::vec3 axis) {

		if (axis.x == 0.0 && axis.y == 0.0 && axis.z == 0.0) return true;

		float amin, bmin, amax, bmax;

		amin = bmin = std::numeric_limits<float>::max();
		amax = bmax = std::numeric_limits<float>::min();
		
		for (int i = 0; i < a.size(); i++) {
			amin = std::min(amin, glm::dot(a.at(i), axis));
			amax = std::max(amax, glm::dot(a.at(i), axis));
			bmin = std::min(bmin, glm::dot(b.at(i), axis));
			bmax = std::max(bmax, glm::dot(b.at(i), axis));
		}

		float combinedLength = (amax - amin) + (bmax - bmin);
		float entireLength = std::max(amax, bmax) - std::min(amin, bmin);
		return combinedLength > entireLength;
	}

	bool detectCollision(BoundingBox other) {
		BoundingBox thisBoundingBox = getTransformedBoundingBox();

		std::vector<glm::vec3> axes;

		glm::vec3 a1 = glm::normalize(thisBoundingBox.v4 - thisBoundingBox.v1);
		glm::vec3 a2 = glm::normalize(thisBoundingBox.v1 - thisBoundingBox.v5);
		glm::vec3 a3 = glm::normalize(thisBoundingBox.v5 - thisBoundingBox.v6);

		glm::vec3 b1 = glm::normalize(other.v4 - other.v1);
		glm::vec3 b2 = glm::normalize(other.v1 - other.v5);
		glm::vec3 b3 = glm::normalize(other.v5 - other.v6);

		axes.push_back(a1);
		axes.push_back(a2);
		axes.push_back(a3);

		axes.push_back(b1);
		axes.push_back(b2);
		axes.push_back(b3);

		axes.push_back(glm::normalize(glm::cross(a1, b1)));
		axes.push_back(glm::normalize(glm::cross(a1, b2)));
		axes.push_back(glm::normalize(glm::cross(a1, b3)));
		axes.push_back(glm::normalize(glm::cross(a2, b1)));
		axes.push_back(glm::normalize(glm::cross(a2, b2)));
		axes.push_back(glm::normalize(glm::cross(a2, b3)));
		axes.push_back(glm::normalize(glm::cross(a3, b1)));
		axes.push_back(glm::normalize(glm::cross(a3, b2)));
		axes.push_back(glm::normalize(glm::cross(a3, b3)));

		for (int i = 0; i < axes.size(); i++) {
			if (!checkAxisProjectionOverlap(convertBoundingBoxToVector(thisBoundingBox), convertBoundingBoxToVector(other), axes.at(i))) {
				return false;
			}
		}

		return true;

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

