#include "Queue.h"


Queue::Queue(int lag) {
	head = 0;
	tail = 0;
	len = 0;
	delay = lag;
}

void Queue::changeDelay(int changed_delay) {
	
	if(delay > 0) {


		glm::mat4 temp[30];


		int rightBound;
		if (changed_delay < len) {
			rightBound = changed_delay;
		}
		else {
			rightBound = len;
		}

		for (int i = 0; i < rightBound; i++) {
			temp[i] = buffer[(head + i) % delay];
		}
		for (int i = 0; i < rightBound; i++) {
			buffer[i] = temp[i];
		}

		head = 0;
		tail = changed_delay > 0 ? (rightBound % changed_delay) : 0;
		len = rightBound;
	}

	delay = changed_delay;


}

void Queue::enqueue(glm::mat4 position) {
	
	buffer[tail] = position;
	len++;
	tail = (tail + 1) % (delay);
	
}

glm::mat4 Queue::dequeue() {
	
	glm::mat4 output = buffer[head];
	head = (head + 1) % (delay);
	len--;
	return output;
	
	
}

glm::mat4 Queue::peek() {
	return buffer[head];
}

int Queue::size() {
	return len;
}

bool Queue::isEmpty() {
	return len == 0;
}
