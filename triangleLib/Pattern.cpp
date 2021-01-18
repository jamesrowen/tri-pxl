#include "Pattern.h"
#include "Arduino.h"
#include "jamesPixelLib.h"

Pattern::Pattern(float min, float max, void(*_update)(float, float, float, float, float, float)) :
  minSpeed(min), maxSpeed(max), update(_update) {
}

Pattern::Pattern() {
}

Pattern::~Pattern() {
}

float Pattern::tick(float progress, long elapsed, float speedFactor, float p1, float p2, float p3, float p4, float p5) {
  progress += elapsed / 1000.0 * ((maxSpeed - minSpeed) * speedFactor + minSpeed);
  progress -= (long) progress;
  update(progress, p1, p2, p3, p4, p5);
  return progress;
}

// uint32_t color1, uint32_t color2, uint32_t color3,
// int width, int num, bool reverse,
// float duration, int minBright, int maxBright, float separation,
