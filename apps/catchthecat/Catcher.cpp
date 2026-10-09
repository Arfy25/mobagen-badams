#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(CatWorld* world) {
  std::vector<Point2D> path = generatePath(world);
  if (path.empty()) {
    return FindHelpfulPoint(world);
  }

  Point2D next = path.front();
  for (auto neighbor : world->neighbors(next)) {
    if (neighbor == world->getCat()) return next;
  }

  std::vector<Point2D> path2 = generatePath(world, next);
  if (path2.empty()) {
    return next;
  }

  Point2D second = path2.front();
  return second;
}

Point2D Catcher::FindHelpfulPoint(CatWorld* world) {
  std::vector<Point2D> neighbors = world->neighbors(world->getCat());
  std::pmr::unordered_map<Point2D, int> score;

  int highestScore = INT_MIN;
  Point2D highestScorePoint = neighbors[0];

  for (Point2D neighbor : neighbors)
  {
    if (world->getContent(neighbor)) continue;

    for (Point2D neighborNeighbors : world->neighbors(neighbor)) {
      if (world->getContent(neighborNeighbors)) {
        score[neighbor]++;
      }
    }
    if (score[neighbor] >= 5) return neighbor;
    if (score[neighbor] > highestScore) {
      highestScore = score[neighbor];
      highestScorePoint = neighbor;
    }
  }

  return highestScorePoint;
}
