// get a color on the rainbow, 0 < x < 1
uint32_t rainbow(float x) {
  x = (int) (255 * (1 - (x - floor(x))));
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

// a more pastel rainbow
uint32_t rainbow2(float x) {
  int red = sin(x * 6.28) * 127 + 128;
  int green = sin(x * 6.28 + 2.09) * 127 + 128;
  int blue = sin(x * 6.28 + 4.19) * 127 + 128;
  return strip.Color(red, green, blue);
}

float easeQuadratic(float t) {
  t *= 2;
  if (t < 1) return t * t / 2.0;
  t--;
  return -(t * (t - 2) - 1) / 2.0;
}
