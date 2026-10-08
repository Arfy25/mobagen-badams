#include "Agent.h"
#include <climits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "World.h"

using namespace std;

std::vector<Point2D> Agent::generatePath(CatWorld* w) {
  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
  queue<Point2D> frontier;                   // to store next ones to visit
  unordered_set<Point2D> frontierSet;        // OPTIMIZATION to check faster if a point is in the queue
  unordered_map<Point2D, bool> visited;      // use .at() to get data, if the element dont exist [] will give you wrong results

  // bootstrap state
  auto catPos = w->getCat();
  frontier.push(catPos);
  frontierSet.insert(catPos);
  Point2D borderExit = {INT32_MAX, INT32_MAX};  // sentinel: no border found yet

  while (!frontier.empty()) {
    // get the current from frontier
    Point2D current = frontier.front();
    frontier.pop();

    // remove the current from frontierset
    frontierSet.erase(current);

    // mark current as visited
    visited[current] = true;

    std::vector<Point2D> neighbors = w->neighbors(current);
    std::vector<Point2D> validNeighbors;

    // getVisitableNeighbors(world, current) returns a vector of neighbors that are not visited, not cat, not block, not in the queue
    for (Point2D neighbor : neighbors) {
      if (w->isValidPosition(neighbor)) {
        if (!visited[neighbor] && w->getCat() != neighbor && !w->getContent(neighbor) && !frontierSet.contains(neighbor)) {

          cameFrom[neighbor] = current;

          if (w->catWinsOnSpace(neighbor)) {
            borderExit = neighbor;
            break;
          }

          frontier.push(neighbor);
          frontierSet.emplace(neighbor);
        }
      }
    }

    // do this up to find a visitable border and break the loop
    if (borderExit != Point2D{INT32_MAX, INT32_MAX}) {
      break;
    }
  }

  // if there isnt a reachable border, just return empty vector
  vector<Point2D> path;

  // if the border is not infinity, build the path from border to the cat using the camefrom map
  if (borderExit != Point2D{INT32_MAX, INT32_MAX}) {
    Point2D current = borderExit;
    path.push_back(current);

    while (cameFrom[current] != w->getCat()) {
      current = cameFrom[current];
      path.push_back(current);
    }
  }

  // if your vector is filled from the border to the cat, the first element is the catcher move, and the last element is the cat move
  return path;
}
