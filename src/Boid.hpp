#ifndef BOID_H
#define BOID_H

#include "glm/fwd.hpp"
#include "p6/p6.h"
#include <cmath>

class Boid {

private:
  glm::vec3 m_position, m_speed, m_direction;
  float triangleSize, leftLimit, rightLimit, topLimit, bottomLimit;

public:
  Boid(p6 ::Context &ctx);
  void drawBoid(p6::Context &ctx);
  void updatePosition();
  bool turnBackOutBorder(float turnBack);
  glm::vec3 targetSpeed(glm::vec3 target, float followSpeed);
  float getDistance(Boid boid1, Boid boid2);
  void follow(const Boid &boid, float followSpeed);
  void separate(Boid boid1, Boid boid2, float avoidDistance);
  void align(const Boid &boid, float alignDistance, float alignStrength);
};

void generateBoids(std::vector<Boid> &myBoids, int boidsNumber,
                   p6::Context &ctx);

#endif