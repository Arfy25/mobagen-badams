#include "HuntAndKillExample.h"
#include "../World.h"
#include "../SeededRandom.h"
#include "Random.h"
#include <climits>
bool HuntAndKillExample::Step(World* w) {
  // todo: code this

  if (stack.empty())
  {
    Point2D newStart = randomStartPoint(w);
    if (newStart == Point2D(INT_MAX, INT_MAX)) { return false; }

    std::vector<Point2D> neighbors = getVisitedNeighbors(w, newStart);
    if (!neighbors.empty())
    {
      Point2D visitedNeighbor = neighbors[SeededRandom::next() % neighbors.size()];
      Point2D worldNewStart = w->ToWorldCoords(newStart);

      if (visitedNeighbor.y < newStart.y)      { w->SetNorth(worldNewStart, false); }
      else if (visitedNeighbor.y > newStart.y) { w->SetSouth(worldNewStart, false); }
      else if (visitedNeighbor.x > newStart.x) { w->SetEast(worldNewStart, false); }
      else if (visitedNeighbor.x < newStart.x) { w->SetWest(worldNewStart, false); }
    }

    stack.push_back(newStart);
  }

  Point2D current = stack.back();
  Point2D worldCurrent = w->ToWorldCoords(current);

  if (!visited[current.x][current.y])
  {
    visited[current.x][current.y] = true;
    w->SetNodeColor(worldCurrent, Color32(0.5, 0, 0, 1));
  }

  std::vector<Point2D> visitables = getVisitables(w, current);

  if (visitables.empty())
  {
    w->SetNodeColor(worldCurrent, Color32(0.5,0,0,1));
    stack.clear();

    return true;
  }

  Point2D newPoint;
  if (visitables.size() == 1)
  {
    newPoint = visitables[0];
  }
  else if (visitables.size() > 1)
  {
    newPoint = visitables[SeededRandom::next() % visitables.size()];
  }

  stack.push_back(newPoint);

  if (newPoint.y < current.y)
  {
    w->SetNorth(worldCurrent, false);
  }
  else if (newPoint.x > current.x)
  {
    w->SetEast(worldCurrent, false);
  }
  else if (newPoint.y > current.y)
  {
    w->SetSouth(worldCurrent, false);
  }
  else if (newPoint.x < current.x)
  {
    w->SetWest(worldCurrent, false);
  }

  return true;
}
void HuntAndKillExample::Clear(World* world) {
  visited.clear();
  stack.clear();

  for (int i = 0; i < world->GetHeight(); i++) {
    for (int j = 0; j < world->GetWidth(); j++) {
      visited[i][j] = false;
    }
  }
}
Point2D HuntAndKillExample::randomStartPoint(World* world) {
  // Todo: improve this if you want
  for (int y = 0; y < world->GetHeight(); y++)
    for (int x = 0; x < world->GetWidth(); x++)
      if (!visited[x][y])
      {
          return {x, y};
      } else
      {
          world->SetNodeColor(world->ToWorldCoords({x,y}), {0,0,0,1});
      }
  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> HuntAndKillExample::getVisitables(World* w, const Point2D& p) {
  std::vector<Point2D> visitables;

  // todo: code this

  Point2D upPoint = Point2D(p.x, p.y - 1);
  Point2D rightPoint = Point2D(p.x + 1, p.y);
  Point2D downPoint = Point2D(p.x, p.y + 1);
  Point2D leftPoint = Point2D(p.x - 1, p.y);

  if (0 <= upPoint.y && upPoint.y < w->GetHeight() && !checkIfVisited(upPoint)) { visitables.push_back(upPoint); }
  if (0 <= rightPoint.x && rightPoint.x < w->GetWidth() && !checkIfVisited(rightPoint)) { visitables.push_back(rightPoint); }
  if (0 <= downPoint.y && downPoint.y < w->GetHeight() && !checkIfVisited(downPoint)) { visitables.push_back(downPoint); }
  if (0 <= leftPoint.x && leftPoint.x < w->GetWidth() && !checkIfVisited(leftPoint)) { visitables.push_back(leftPoint); }

  return visitables;
}

std::vector<Point2D> HuntAndKillExample::getVisitedNeighbors(World* w, const Point2D& p) {
  std::vector<Point2D> deltas = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
  std::vector<Point2D> neighbors;

  // todo: code this

  for (auto & delta : deltas)
  {
    Point2D neighbor = {p.x + delta.x, p.y + delta.y};

    if (neighbor.x >= 0 && neighbor.y >= 0 && neighbor.x < w->GetWidth() && neighbor.y < w->GetHeight())
    {
      if (checkIfVisited(neighbor))
      {
        neighbors.push_back(neighbor);
      }
    }
  }

  return neighbors;
}

bool inline HuntAndKillExample::checkIfVisited(Point2D point)
{
  if (visited.contains(point.x))
  {
    if (visited[point.x].contains(point.y))
    {
      return visited[point.x][point.y];
    }
  }
  return false;
}
