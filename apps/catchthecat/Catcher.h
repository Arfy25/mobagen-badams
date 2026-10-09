#ifndef CATCHER_H
#define CATCHER_H

#include "Agent.h"

class Catcher : public Agent {
public:
  explicit Catcher() : Agent(){};
  Point2D Move(CatWorld*) override;
  Point2D FindHelpfulPoint(CatWorld*);
};

#endif  // CATCHER_H
