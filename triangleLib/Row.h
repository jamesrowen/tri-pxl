#ifndef TRIANGLE_ROW_H
#define TRIANGLE_ROW_H

#include "Arduino.h"
#include "jamesPixelLib.h"

class Row {

 public:
  Row(Adafruit_NeoPixel& _leds, uint8_t _start, uint8_t _length);
  Row(void);
  ~Row();

  void
    setLeft(float percentage, bool direction, uint32_t color, uint8_t brightness),
    setRight(float percentage, bool direction, uint32_t color, uint8_t brightness),
    setBottom(float percentage, bool direction, uint32_t color, uint8_t brightness),
    setAll(float percentage, bool direction, uint32_t color, uint8_t brightness),
    setPulse(float progress, uint8_t num, uint8_t width, bool direction, float startColor,
      uint8_t numColors, float colorOffset, uint8_t brightness),
    setPulse(float progress, uint8_t num, bool direction, float startColor,
      uint8_t numColors, float colorOffset, uint8_t brightness, float width);
 private:
  uint32_t rainbow(float x);
  uint8_t length, offset, start, end;
  Adafruit_NeoPixel leds;
  int main_brightness = 80;
};

#endif // TRIANGLE_ROW_H
