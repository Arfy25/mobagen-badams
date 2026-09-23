#include "CohesionRule.h"
#include <glm/glm.hpp>

glm::vec2 CohesionRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 cohesionForce(0.f);

  // glm::length(vec) returns the length of a vector,
  // glm::normalize(vec) returns the normalized vector (length 1) in the same direction as vec.

  // begin solution

  if (neighborhood.empty()) { return cohesionForce; }

  glm::vec2 centerOfMass(0.f);
  int count = neighborhood.size();

  // Just reread the gameguild and saw that cohesion excludes the boid for center of mass
  for (int i = 0; i < neighborhood.size(); i++)
  {
    centerOfMass += neighborhood[i].position;
  }

  centerOfMass /= static_cast<float>(count);

  glm::vec2 direction = centerOfMass - boid.position;
  float distance = glm::length(direction);

  if (distance > 0.001f)
  {
    cohesionForce = direction / distance;
  }

  // end solution

  return cohesionForce;
}
