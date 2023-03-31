#include "Boid.hpp"
#include "glm/ext/quaternion_geometric.hpp"

// My Constructor
Boid::Boid(p6::Context &ctx) {

  leftLimit = -ctx.aspect_ratio();
  rightLimit = ctx.aspect_ratio();
  topLimit = -1;
  bottomLimit = 1;

  position = glm::vec3(p6::random::number(leftLimit, rightLimit),
                       p6::random::number(topLimit, bottomLimit), 0);
  speed = glm::vec3(p6::random::number(-0.5, 0.5),
                    p6::random::number(-0.5, 0.05), (0.0)) *
          0.01f;
  radius = 0.05f;
}

// My Canvas border
bool Boid::canvasBorders(p6::Context &ctx) {

  if (position.x < leftLimit || position.x > rightLimit ||
      position.y < topLimit || position.y > bottomLimit) {
    return false;
  }
  return true;
}

// Reset myBoids

// Generate Boids
void generateBoids(std::vector<Boid> &myBoids, int boidsNumber,
                   p6::Context &ctx) {
  for (int i = 0; i < boidsNumber; i++) {
    myBoids.emplace_back(Boid(ctx));
  }
};

// Draw my Boid
void Boid::drawBoid(p6::Context &ctx) {
  ctx.fill = {p6::NamedColor::Salmon};
  ctx.circle(p6::Center{position}, p6::Radius{radius});
};

// New position + turn back on borders
void Boid::updatePosition() { position += speed; }
void Boid::turnBack() { speed = -speed; }

// get the speed and direction of the boid I follow
glm::vec3 Boid::targetSpeed(glm::vec3 target, float followSpeed) {
  glm::vec3 follower = target - position;
  return follower * followSpeed;
}

// follow other boids when we meet an other
void Boid::follow(const Boid &boid, float followSpeed) {
  glm::vec3 target = boid.position;
  speed = speed + targetSpeed(target, followSpeed);
  if (glm::length(speed) > 0.01) {
    speed = glm::normalize(speed) * 0.001f;
  }
}

// get the distance between 2 boids
float Boid::getDistance(Boid boid1, Boid boid2) {
  return glm::distance(boid1.position, boid2.position);
}

// avoid the other boids when too closed
// avoidDistance : min distance between 2 boids
glm::vec3 Boid::avoidBoids(Boid boid1, Boid boid2, float avoidDistance) {
  float distanceBetweenBoids = getDistance(boid1, boid2);
  if (distanceBetweenBoids < avoidDistance) {
    glm::vec3 avoidVector = glm::normalize(boid1.position - boid2.position) *
                            (avoidDistance - distanceBetweenBoids);
    return avoidVector;
  } else {
    // not too closed -> vec avoidVec=0
    return glm::vec3(0.0f);
  }
}

// faire gui
//  changer mes ronds en rectangle