#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(CatWorld* world) {
  std::vector<Point2D> path = generatePath(world);
  if (path.empty()) {
    return FindOpenSpaceNearby(world);
  }
  Point2D next = path.back();
  return next;
}

Point2D Cat::FindOpenSpaceNearby(CatWorld* world) {

  std::vector<Point2D> neighbors = world->neighbors(world->getCat());
  std::pmr::unordered_map<Point2D, int> score;

  int lowestScore = INT_MAX;
  Point2D lowestScorePoint = neighbors[0];

  for (Point2D neighbor : neighbors)
  {
    if (world->getContent(neighbor)) continue;

    for (Point2D neighborNeighbors : world->neighbors(neighbor)) {
      if (world->getContent(neighborNeighbors)) {
        score[neighbor]++;
      }
    }
    if (score[neighbor] <= 0) return neighbor;
    if (score[neighbor] < lowestScore) {
      lowestScore = score[neighbor];
      lowestScorePoint = neighbor;
    }
  }

  return lowestScorePoint;
}
