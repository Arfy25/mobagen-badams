#include "../World.h"
#include "../SeededRandom.h"
#include "RecursiveBacktrackerExample.h"
#include <climits>

// Recursive backtracker, in FORMAL units: (0, 0) is the top-left cell, x grows
// right, y grows down. The caller seeds SeededRandom before the first Step;
// every decision consumes the seed in order, so the maze is deterministic.
//
// Procedure per Step, on the cell at the top of the path stack:
//   1. mark it visited;
//   2. list its visitable (unvisited) neighbors in clockwise order starting
//      from the top: UP, RIGHT, DOWN, LEFT (getVisitables does this);
//   3. none        -> dead end: pop the stack (backtrack). Empty stack = done;
//   4. exactly one -> move to it, do not consume a random number;
//   5. two or more -> consume SeededRandom::next() and pick
//      next() % visitableCount;
//   6. moving opens the wall between the two cells
//      (World::SetNorth/SetEast/SetSouth/SetWest with false).

void RecursiveBacktrackerExample::Clear(World* world) {
  // todo: reset the walk
  // hint:
  //   clear visited and the path stack, then start the walk at the
  //   top-left cell in formal units: stack.push_back({0, 0})
  // begin solution

  visited.clear();
  stack.clear();

  stack.push_back({0,0});

  // end solution
}

bool RecursiveBacktrackerExample::Step(World* w) {
  // todo: implement one iteration of the recursive backtracker
  // hint:
  //   empty stack  -> the maze is done, return false
  //   otherwise, on the cell at the top of the stack (formal units):
  //   1. mark it visited;
  //   2. list its visitable neighbors with getVisitables
  //      (already in clockwise order: UP, RIGHT, DOWN, LEFT);
  //   3. none        -> dead end: pop the stack (backtrack);
  //   4. exactly one -> move to it, do not consume a random number;
  //   5. two or more -> consume SeededRandom::next() and pick
  //      next() % visitables.size();
  //   moving = opening the wall between the two cells, through the
  //   World coordinate translation:
  //     Point2D worldCurrent = w->ToWorldCoords(current);
  //     UP    -> w->SetNorth(worldCurrent, false)
  //     RIGHT -> w->SetEast(worldCurrent, false)
  //     DOWN  -> w->SetSouth(worldCurrent, false)
  //     LEFT  -> w->SetWest(worldCurrent, false)
  //   return true while there is still work (stack not empty after the move)
  // begin solution

    if (stack.empty()) { return false; }

    Point2D current = stack.back();
    Point2D worldCurrent = w->ToWorldCoords(current);

    if (visited[current.x][current.y])
    {
        w->SetNodeColor(worldCurrent, Color32(0,0,0,1));
    }
    else
    {
        visited[current.x][current.y] = true;
        w->SetNodeColor(worldCurrent, Color32(0.5,0,0,1));
    }

    std::vector<Point2D> visitables = getVisitables(w, current);

    if (visitables.empty()) {
        w->SetNodeColor(worldCurrent, Color32(0,0,0,1));
        stack.pop_back();
        return !stack.empty();
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

  // end solution
  return false;
}

std::vector<Point2D> RecursiveBacktrackerExample::getVisitables(World* w, const Point2D& formalPoint) {
  // todo: list the unvisited neighbors of formalPoint, in clockwise order
  // hint:
  //   candidates in order: UP {x, y-1}, RIGHT {x+1, y}, DOWN {x, y+1}, LEFT {x-1, y}
  //   keep a candidate only if it is inside the grid
  //   (0 <= x < w->GetWidth(), 0 <= y < w->GetHeight()) and not visited
  // begin solution

  std::vector<Point2D> visitables = std::vector<Point2D>();

  Point2D upPoint = Point2D(formalPoint.x, formalPoint.y - 1);
  Point2D rightPoint = Point2D(formalPoint.x + 1, formalPoint.y);
  Point2D downPoint = Point2D(formalPoint.x, formalPoint.y + 1);
  Point2D leftPoint = Point2D(formalPoint.x - 1, formalPoint.y);

  if (0 <= upPoint.y && upPoint.y < w->GetHeight() && !checkIfVisited(upPoint)) { visitables.push_back(upPoint); }
  if (0 <= rightPoint.x && rightPoint.x < w->GetWidth() && !checkIfVisited(rightPoint)) { visitables.push_back(rightPoint); }
  if (0 <= downPoint.y && downPoint.y < w->GetHeight() && !checkIfVisited(downPoint)) { visitables.push_back(downPoint); }
  if (0 <= leftPoint.x && leftPoint.x < w->GetWidth() && !checkIfVisited(leftPoint)) { visitables.push_back(leftPoint); }

  // end solution
  return visitables;
}

bool inline RecursiveBacktrackerExample::checkIfVisited(Point2D point)
{
    if (visited.contains(point.x))
    {
        if (visited[point.x].contains(point.y))
        {
            if (visited[point.x][point.y] == true)
            {
                return true;
            }
        }
    }
    return false;
}
