#ifndef BOID_H
#define BOID_H

#include "glm/fwd.hpp"
#include "p6/p6.h"
#include <cmath>

class Boid {

private:
  glm::vec3 position, speed;
  float radius, leftLimit, rightLimit, topLimit, bottomLimit;

public:
  Boid(p6 ::Context &ctx);
  bool canvasBorders(p6::Context &ctx);
  void drawBoid(p6::Context &ctx);
  void updatePosition();
  void turnBack();
  void follow(const Boid &boid);
  glm::vec3 targetSpeed(glm::vec3 target);
  float getDistance(Boid boid1, Boid boid2);
  glm::vec3 avoidBoids(Boid boid1, Boid boid2, float avoidDistance);
};

#endif