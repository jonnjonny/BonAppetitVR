#ifndef QUEUE_H
#define QUEUE_H

// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class Queue {
public:
  Queue(int lag);

  glm::mat4 buffer [30];
  int head;
  int tail;
  int len;
  int delay;

  void changeDelay(int changed_delay);
  void enqueue(glm::mat4 controllerPosition);
  glm::mat4 dequeue();
  glm::mat4 peek();
  int size();
  bool isEmpty();
  

};

#endif
