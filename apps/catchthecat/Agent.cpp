#include "Agent.h"
#include <climits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "World.h"

using namespace std;

struct ComparePointScore {
  bool operator()(const std::pair<int, Point2D>& a, const std::pair<int, Point2D>& b) const {
    return a.first > b.first;
  }
};

std::vector<Point2D> Agent::generatePath(CatWorld* w) {
  typedef std::pair<int, Point2D> PointScore;

  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
  unordered_map<Point2D, int> score;         // score for the current point
  std::priority_queue<PointScore, std::vector<PointScore>, ComparePointScore> frontier;   // A priority queue that sorts based on the best score
  unordered_map<Point2D, bool> visited;      // use .at() to get data, if the element dont exist [] will give you wrong results

  // bootstrap state
  auto catPos = w->getCat();
  score[catPos] = 0;

  int initialFScore = 0 + heuristic(w, catPos);
  frontier.push({initialFScore, catPos});

  Point2D borderExit = {INT32_MAX, INT32_MAX};  // sentinel: no border found yet

  while (!frontier.empty()) {
    // get the current from frontier
    Point2D current = frontier.top().second;
    frontier.pop();

    // mark current as visited
    if (visited[current]) continue;
    visited[current] = true;

    if (w->catWinsOnSpace(current)) {
      borderExit = current;
      break;
    }

    std::vector<Point2D> neighbors = w->neighbors(current);

    // getVisitableNeighbors(world, current) returns a vector of neighbors that are not visited, not cat, not block, not in the queue
    for (Point2D neighbor : neighbors) {
      if (w->isValidPosition(neighbor)) {
        if (w->getCat() == neighbor || w->getContent(neighbor)) continue;

        int possibleScore = score[current] + 1;

        if (score.find(neighbor) == score.end() || possibleScore < score[neighbor]) {
          cameFrom[neighbor] = current;
          score[neighbor] = possibleScore;

          int fScore = possibleScore + heuristic(w, neighbor);
          frontier.push({fScore, neighbor});
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

int Agent::heuristic(CatWorld* w, Point2D p) {
  int size = w->getWorldSideSize();

  int distLeft = p.x;
  int distRight = (size - 1) - p.x;
  int distTop = p.y;
  int distBottom = (size - 1) - p.y;

  return std::min({distLeft, distRight, distTop, distBottom});
}
