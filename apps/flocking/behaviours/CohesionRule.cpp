#include "CohesionRule.h"
#include <glm/glm.hpp>

glm::vec2 CohesionRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 cohesionForce(0.f);

  // glm::length(vec) returns the length of a vector,
  // glm::normalize(vec) returns the normalized vector (length 1) in the same direction as vec.

  // begin solution
  glm::vec2 centerOfMass(0.f);
  int count = neighborhood.size() + 1;

  for (int i = 0; i < neighborhood.size(); i++)
  {
    centerOfMass += neighborhood[i].position;
  }

  centerOfMass += boid.position;
  centerOfMass /= count;

  glm::vec2 direction = centerOfMass - boid.position;
  if (glm::length(direction) > 0.f)
  {
    cohesionForce = glm::normalize(direction);
  }
  else
  {
    cohesionForce = glm::vec2(0.f);
  }

  // end solution

  return cohesionForce;
}
