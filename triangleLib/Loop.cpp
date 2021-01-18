#include "Loop.h"
#include "Arduino.h"
#include "jamesPixelLib.h"

Loop::Loop(Pattern* p, int i, bool r, float ss, float se, float _p1s, float _p1e, float _p2s, float _p2e, float _p3s, float _p3e, float _p4s, float _p4e, float _p5s, float _p5e) :
  pattern(p), iterations(i), doReverse(r), speedStart(ss), speedEnd(se), p1s(_p1s), p1e(_p1e), p2s(_p2s), p2e(_p2e), p3s(_p3s), p3e(_p3e), p4s(_p4s), p4e(_p4e), p5s(_p5s), p5e(_p5e) {
}

Loop::Loop() {
}

Loop::~Loop() {
}

void Loop::start() {
  completedIterations = 0;
  reversing = false;
}

bool Loop::tick(long elapsed) {
  float lastProgress = patternProgress;
  float x = (completedIterations + patternProgress) / iterations;
  if (reversing) {
    x = 1 - x;
  }
  patternProgress = pattern->tick(patternProgress, elapsed, lerp(x, speedStart, speedEnd), lerp(x, p1s, p1e), lerp(x, p2s, p2e), lerp(x, p3s, p3e), lerp(x, p4s, p4e), lerp(x, p5s, p5e));
  if (patternProgress < lastProgress) {
    completedIterations++;
    if (completedIterations >= iterations && doReverse && !reversing) {
      reversing = true;
      completedIterations = 0;
    }
  }
  return completedIterations >= iterations;
}

float Loop::lerp(float progress, float start, float end) {
  return progress * (end - start) + start;
}

// uint32_t color1, uint32_t color2, uint32_t color3,
// int width, int num, bool reverse,
// float duration, int minBright, int maxBright, float separation,
