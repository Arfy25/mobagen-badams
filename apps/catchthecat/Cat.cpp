#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(CatWorld* world) {
  if (generatePath(world).empty()) {
    return world->getCat();
  }
  Point2D next = generatePath(world).back();
  return next;
}
