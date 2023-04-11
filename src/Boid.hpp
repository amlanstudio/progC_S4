#ifndef BOID_H
#define BOID_H

#include "glm/fwd.hpp"
#include "p6/p6.h"
#include <cmath>

class Boid {

private:
  glm::vec3 position, speed, direction;
  float radius, leftLimit, rightLimit, topLimit, bottomLimit;

public:
  Boid(p6 ::Context &ctx);
  bool canvasBorders(p6::Context &ctx);
  void drawBoid(p6::Context &ctx);
  void updatePosition();
  void turnBack();
  void follow(const Boid &boid, float followSpeed);
  glm::vec3 targetSpeed(glm::vec3 target, float followSpeed);
  float getDistance(Boid boid1, Boid boid2);
  glm::vec3 avoidBoids(Boid boid1, Boid boid2, float avoidDistance);
};

void generateBoids(std::vector<Boid> &myBoids, int boidsNumber,
                   p6::Context &ctx);

#endif