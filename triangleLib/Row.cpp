#include "Row.h"
#include "Arduino.h"
#include "jamesPixelLib.h"

Row::Row(Adafruit_NeoPixel& _leds, uint8_t _start, uint8_t _length) :
  leds(_leds), start(_start), length(_length) {
}

Row::Row() {
}

Row::~Row() {
}

int mod(int x, int m) {
    int r = x%m;
    return r<0 ? r+m : r;
}

void Row::setLeft(float percentage, bool direction, uint32_t color, uint8_t brightness) {
  for (int i=0; i<ceil((length / 3 + 1) * percentage); i++) {
    leds.setPixel(direction ? (start + mod(length / 3 - 1 - i, length)) : (start + mod(i - 1, length)), color, brightness);
  }
}

void Row::setRight(float percentage, bool direction, uint32_t color, uint8_t brightness) {
  for (int i=0; i<ceil((length / 3 + 1) * percentage); i++) {
    leds.setPixel(direction ? (start + length / 3 * 2 - 1 - i) : (start + length / 3 + i - 1), color, brightness);
  }
}

void Row::setBottom(float percentage, bool direction, uint32_t color, uint8_t brightness) {
  for (int i=0; i<ceil((length / 3 + 1) * percentage); i++) {
    leds.setPixel(direction ? (start + length - 1 - i) : (start + length / 3 * 2 + i - 1), color, brightness);
  }
}

void Row::setAll(float percentage, bool direction, uint32_t color, uint8_t brightness) {
  for (int i=0; i<round(length * percentage); i++) {
    leds.setPixel(direction ? (start + length - i - 1) : (i + start), color, brightness);
  }
}


void Row::setPulse(float progress, uint8_t num, uint8_t width, bool direction, float startColor,
  uint8_t numColors, float colorOffset, uint8_t brightness) {
  for (int j=0; j<num; ++j)
    for (int i=0; i<width; ++i) {
      int a = (int)(floor(progress * length) + i + round(j * length / (float) num)) % length;
      if (direction) {
        a = length - a;
      }
      leds.setPixel(a + start, rainbow(numColors <= 1 ? startColor : startColor + colorOffset * (j % numColors) / (float)(numColors - 1)), brightness);
    }
}
void Row::setPulse(float progress, uint8_t num, bool direction, float startColor,
  uint8_t numColors, float colorOffset, uint8_t brightness, float sizeInPercent) {
  setPulse(progress, num, sizeInPercent / 100.0 * length, direction, startColor, numColors, colorOffset, brightness);
}

uint32_t Row::rainbow(float x) {
  x = (int) (255 * (1 - (x - floor(x))));
  if(x < 85) {
    return (255 - x * 3) + (0 * 256) + (x * 3 * 256 * 256);
  }
  if(x < 170) {
    x -= 85;
    return 0 + (x * 3 * 256) + ((255 - x * 3) * 256 * 256);
  }
  x -= 170;
  return (x * 3) + ((255 - x * 3) * 256) + (0 * 256 * 256);
}
