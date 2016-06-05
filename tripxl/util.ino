// input: 0 to 255
// returns: a color along the transition r - g - b
uint32_t rainbow(byte x) {
  x = 255 - x;
  if(x < 85) {
    return strip.Color(255 - x * 3, 0, x * 3);
  }
  if(x < 170) {
    x -= 85;
    return strip.Color(0, x * 3, 255 - x * 3);
  }
  x -= 170;
  return strip.Color(x * 3, 255 - x * 3, 0);
}

float easeQuadratic(float t) {
  t *= 2;
  if (t < 1) return t * t / 2.0;
  t--;
  return -(t * (t - 2) - 1) / 2.0;
}

int readB() {
  return (bool) Serial.parseInt();
}
int readI() {
  return Serial.parseInt();
}
float readF() {
  return Serial.parseFloat();
}
