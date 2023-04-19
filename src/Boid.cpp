#include "Boid.hpp"
#include "glm/ext/quaternion_geometric.hpp"

// My Constructor
Boid::Boid(p6::Context &ctx) {

  leftLimit = -ctx.aspect_ratio();
  rightLimit = ctx.aspect_ratio();
  topLimit = -1;
  bottomLimit = 1;

  m_position = glm::vec3(p6::random::number(leftLimit, rightLimit),
                         p6::random::number(topLimit, bottomLimit), 0);
  m_speed = glm::vec3(p6::random::number(-0.5, 0.5),
                      p6::random::number(-0.5, 0.05), (0.0)) *
            0.01f;
  triangleSize = 0.05f;
  m_direction = glm::vec3(p6::random::number(-0.5, 0.5),
                          p6::random::number(-0.5, 0.5), (0.0));
}

bool Boid::turnBackOutBorder(float turnBack) {
  bool outBorders = true;

  if (m_position.y < topLimit + triangleSize) {
    m_position.y = topLimit + triangleSize;
    m_direction.y = -m_direction.y * turnBack;
    m_direction.x += turnBack * (1 - abs(m_direction.y));
    outBorders = false;
  } else if (m_position.y > bottomLimit - triangleSize) {
    m_position.y = bottomLimit - triangleSize;
    m_direction.y = -m_direction.y * turnBack;
    m_direction.x -= turnBack * (1 - abs(m_direction.y));
    outBorders = false;
  }
  if (m_position.x > rightLimit - triangleSize) {
    m_position.x = rightLimit - triangleSize;
    m_direction.x = -m_direction.x * turnBack;
    m_direction.y += turnBack * (1 - abs(m_direction.x));
    outBorders = false;
  } else if (m_position.x < leftLimit + triangleSize) {
    m_position.x = leftLimit + triangleSize;
    m_direction.x = -m_direction.x * turnBack;
    m_direction.y -= turnBack * (1 - abs(m_direction.x));
    outBorders = false;
  }
  m_direction = glm::normalize(m_direction);
  return outBorders;
}

// Generate Boids
void generateBoids(std::vector<Boid> &myBoids, int boidsNumber,
                   p6::Context &ctx) {
  myBoids.clear();
  for (int i = 0; i < boidsNumber; i++) {
    myBoids.emplace_back(Boid(ctx));
  }
};

// Draw my Boid
void Boid::drawBoid(p6::Context &ctx) {
  ctx.fill = {p6::NamedColor::Salmon};
  ctx.equilateral_triangle(p6::Center{m_position}, p6::Radius{triangleSize},
                           p6::Rotation{m_speed});
};

// New position
void Boid::updatePosition() { m_position += m_speed; }

// get the speed and direction of the boid I follow
glm::vec3 Boid::targetSpeed(glm::vec3 target, float followSpeed) {
  glm::vec3 follower = target - m_position;
  return follower * followSpeed;
}

// follow other boids when we meet an other
void Boid::follow(const Boid &boid, float followSpeed) {
  glm::vec3 target = boid.m_position;
  m_speed = m_speed + targetSpeed(target, followSpeed);
  if (glm::length(m_speed) > 0.01) {
    m_speed = glm::normalize(m_speed) * 0.001f;
  }
}

// get the distance between 2 boids
float Boid::getDistance(Boid boid1, Boid boid2) {
  return glm::distance(boid1.m_position, boid2.m_position);
}

// avoid the other boids when too closed
// avoidDistance : min distance between 2 boids
void Boid::separate(Boid boid1, Boid boid2, float avoidDistance) {
  float distanceBetweenBoids = getDistance(boid1, boid2);
  if (distanceBetweenBoids < avoidDistance) {
    glm::vec3 avoidVector =
        glm::normalize(boid1.m_position - boid2.m_position) *
        (avoidDistance - distanceBetweenBoids);
    m_speed += avoidVector;
  }
}

// get the alignment of boid I follow

void Boid::align(const Boid &boid, float alignDistance, float alignStrength) {
  glm::vec3 averageDirection(0.0f);
  int neighborCount = 0;

  const float distance = glm::distance(m_position, boid.m_position);
  if (distance < alignDistance) {
    averageDirection += boid.m_direction;
    neighborCount++;
  }

  if (neighborCount > 0) {
    averageDirection /= static_cast<float>(neighborCount);
    m_direction += alignStrength * (averageDirection - m_direction);
    m_direction = glm::normalize(m_direction);
  }
}
