#include "AlignmentRule.h"
#include <glm/glm.hpp>

glm::vec2 AlignmentRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 averageVelocity(0.f);
  // glm::vec2 can be divided by a float, which will divide each component of the vector by that float.

  // begin solution
  for (int i = 0; i < neighborhood.size(); i++)
  {
      averageVelocity += neighborhood[i].velocity;
  }

  averageVelocity += boid.velocity;
  averageVelocity /= (neighborhood.size() + 1);

  return averageVelocity;
  // end solution
}
