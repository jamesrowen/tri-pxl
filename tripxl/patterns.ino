// number of rows on, rainbow color [0-1], brightness difference, nothing, nothing
void radiate(float progress, float numRows, float brightAmplitude, float baseColor, float colorOffset, float numColors) {
  int startRow = (int) (progress * NUM_ROWS);
  int fadeAmount = 0;
  int curRow = startRow;
  float color = 0;
  numColors = numColors * NUM_ROWS;
  for (int i=0; i < (int) (numRows * NUM_ROWS) + 1; ++i) {
    curRow = (startRow + i) % NUM_ROWS;
    color = numColors <= 1 ? baseColor : baseColor + colorOffset * (i % (int)numColors) / ((int)numColors - 1);
//    bright = max(brightness - (int)((sin(6.283 * (curRow / (float)NUM_ROWS - progress)) + 1) * .65 * brightness * brightAmplitudc), 0);
    fadeAmount = (sin(6.283 * (curRow / (float)NUM_ROWS - progress)) + 1) * .5 * brightAmplitude * 255;
    setRow(curRow, 1, rainbow(color), fadeAmount, false);
  }
}

// number of chasers, size of chasers, baseColor, colorOffset
void chaser(float progress, float num, float sizeParam, float baseColor, float colorOffset, float numColors) {
  int numChasers = (int) (num * NUM_LEDS + 1);
  int chaserSize = (int) (sizeParam * 8) + 1;
  int startPoint = (int) (progress * NUM_LEDS);
  numColors = min(numChasers, numColors * NUM_LEDS / 2.0);
  for (int j = 0; j < numChasers; ++j) {
    for (int i = 0; i < chaserSize; ++i) {
      int index = (startPoint + (i - chaserSize / 2) + round(j / (float) numChasers * NUM_LEDS)) % NUM_LEDS;
      leds[index] = rainbow(numColors <= 1 ? baseColor : baseColor + colorOffset * (j % (int)numColors) / (numColors - 1));
    }
  }
}

// number of blades, width of blades, color, colorOffset
// set width as a number of lights
void spinner(float progress, float num, float width, float color, float colorOffset, float numColors) {
  int numBlades = (int) (num * 10) + 1;
  float off = 0;
  numColors = numColors * 10;
  for (int i = 0; i < NUM_ROWS; ++i) {
    int bladeWidth = (.02 + width * .98) * ROW_SIZES[i];
    for (int j = 0; j < numBlades; ++j)
      for (int k = 0; k < bladeWidth; ++k) {
        int a = (int)(floor(progress * ROW_SIZES[i]) + k + round(j * ROW_SIZES[i] / (float) numBlades)) % ROW_SIZES[i];
        leds[a + ROW_STARTS[i]] = rainbow(numColors <= 1 ? color : color + colorOffset * (j % (int)numColors) / (float)(numColors - 1));
      }
  }
}

void rainbowRotate(float progress, float rainbowFunc, float p2, float p3, float p4, float p5) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    float x = progress + i / (float) NUM_LEDS;
    leds[i] = rainbowFunc > .5 ? rainbow2(x) : rainbow(x);
  }
}

void fillAllRows(float progress, float p1, float p2, float baseColor, float colorOffset, float p5) {
  for (int i = 0; i < NUM_ROWS; ++i) {
    setRow(i, -2 * abs(progress - .5) + 1, rainbow(baseColor + i * colorOffset), 0, i % 2 == 0 ? (progress > .5 ? true : false) : (progress > .5 ? false : true));
  }
}
