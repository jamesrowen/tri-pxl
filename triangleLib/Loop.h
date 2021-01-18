#ifndef TRIANGLE_LOOP_H
#define TRIANGLE_LOOP_H

#include "Arduino.h"
#include "Pattern.h"

class Loop {

 public:
  Loop(Pattern* p, int i, bool r, float ss, float se, float _p1s, float _p1e, float _p2s, float _p2e, float _p3s, float _p3e, float _p4s, float _p4e, float _p5s, float _p5e);
  Loop();
  ~Loop();

  void start();
  bool tick(long elapsed);

  Pattern* pattern;
  int iterations, completedIterations = 0;
  float patternProgress = 0.0;
  bool doReverse, reversing = false;
  float speedStart, speedEnd, p1s, p1e, p2s, p2e, p3s, p3e, p4s, p4e, p5s, p5e;
  float lerp(float progress, float start, float end);
};

#endif
