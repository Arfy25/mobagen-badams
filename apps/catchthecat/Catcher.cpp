#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(CatWorld* world) {
  Point2D next = generatePath(world).front();
  return next;
}
