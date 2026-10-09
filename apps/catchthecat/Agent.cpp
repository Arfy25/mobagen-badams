#include "Agent.h"
#include <climits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "World.h"

using namespace std;

// Special comparison operator for priority queue
struct ComparePointScore {
  bool operator()(const std::pair<int, Point2D>& a, const std::pair<int, Point2D>& b) const {
    return a.first > b.first;
  }
};

std::vector<Point2D> Agent::generatePath(CatWorld* w, Point2D b) {
  typedef std::pair<int, Point2D> PointScore;

  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
  unordered_map<Point2D, int> score;         // score for the current point
  std::priority_queue<PointScore, std::vector<PointScore>, ComparePointScore> frontier;   // A priority queue that sorts based on the best score
  unordered_map<Point2D, bool> visited;      // use .at() to get data, if the element dont exist [] will give you wrong results

  // Set up inital position and score
  auto catPos = w->getCat();
  score[catPos] = 0;
  int initialScore = 0 + heuristic(w, catPos);
  frontier.push({initialScore, catPos});

  Point2D borderExit = {INT32_MAX, INT32_MAX};  // No border found at start

  while (!frontier.empty()) {

    // Get Current From Frontier
    Point2D current = frontier.top().second;
    frontier.pop();

    // Make the current point visited
    if (visited[current]) continue;
    visited[current] = true;

    // Set the border exit if the cat can win on current
    if (w->catWinsOnSpace(current)) {
      borderExit = current;
      break;
    }

    std::vector<Point2D> neighbors = w->neighbors(current);

    // Get the score of neighbors
    for (Point2D neighbor : neighbors) {
      if (w->isValidPosition(neighbor)) {
        if (w->getCat() == neighbor || w->getContent(neighbor) || neighbor == b) continue;

        // Best possible score
        int currentScore = score[current] + 1;

        if (score.find(neighbor) == score.end() || currentScore < score[neighbor]) {
          // Set the path back
          cameFrom[neighbor] = current;
          score[neighbor] = currentScore;

          frontier.push({currentScore + heuristic(w, neighbor), neighbor});
        }
      }
    }

    // If we found a border exit the loop
    if (borderExit != Point2D{INT32_MAX, INT32_MAX}) {
      break;
    }
  }

  vector<Point2D> path;

  // If we found a border create the path back
  if (borderExit != Point2D{INT32_MAX, INT32_MAX}) {
    Point2D current = borderExit;
    path.push_back(current);

    while (cameFrom[current] != w->getCat()) {
      current = cameFrom[current];
      path.push_back(current);
    }
  }

  return path;
}

int Agent::heuristic(CatWorld* w, Point2D p) {
  int size = w->getWorldSideSize();

  return min({(size/2 - abs(p.x)), (size/2 - abs(p.y))});
}
