#ifndef TRIANGLE_PATTERN_H
#define TRIANGLE_PATTERN_H

#include "Arduino.h"
#include "jamesPixelLib.h"

class Pattern {

 public:
  Pattern(float min, float max, void(*_update)(float, float, float, float, float, float));
  Pattern(void);
  ~Pattern();

  float tick(float progress, long elapsed, float speedFactor, float p1, float p2, float p3, float p4, float p5);

 private:
  float minSpeed, maxSpeed;
  void (*update)(float progress, float p1, float p2, float p3, float p4, float p5);
};

#endif
