// number of rows on, rainbow color [0-1], brightness difference, nothing, nothing
void radiate(float progress, float numRows, float brightAmplitude, float baseColor, float colorOffset, float numColors) {
  int startRow = (int) (progress * NUM_ROWS);
  int bright = 0;
  int curRow = startRow;
  float color = 0;
  numColors = numColors * NUM_ROWS;
  for (int i=0; i < (int) (numRows * NUM_ROWS) + 1; ++i) {
    curRow = (startRow + i) % NUM_ROWS;
    color = numColors <= 1 ? baseColor : baseColor + colorOffset * (i % (int)numColors) / ((int)numColors - 1);
    bright = max(brightness - (int)((sin(6.283 * (curRow / (float)NUM_ROWS - progress)) + 1) * .65 * brightness * brightAmplitude), 0);
    rows[curRow]->setAll(1, false, rainbow(color), bright);
  }
}

// number of chasers, size of chasers, baseColor, colorOffset
void chaser(float progress, float num, float sizeParam, float baseColor, float colorOffset, float numColors) {
  int numChasers = (int) (num * NUM_PIXELS + 1);
  int chaserSize = (int) (sizeParam * 8) + 1;
  int startPoint = BASE_OFFSET + (int) (progress * NUM_PIXELS);
  numColors = min(numChasers, numColors * NUM_PIXELS / 2.0);
  for (int j=0; j<numChasers; ++j) {
    for (int i=0; i<chaserSize; ++i) {
      strip.setPixel(
        (startPoint + (i - chaserSize / 2) + round(j / (float) numChasers * NUM_PIXELS)) % NUM_PIXELS,
        rainbow(numColors <= 1 ? baseColor : baseColor + colorOffset * (j % (int)numColors) / (numColors - 1)),
        brightness
      );
    }
  }
}

// TODO: implement this - segmented snake so the colors go more sequentially
void snake(float progress, float num, float width, float baseColor, float colorOffset, float p5) {
  
}


// number of blades, width of blades, color, colorOffset
// set width as a number of lights
void spinner(float progress, float num, float width, float color, float colorOffset, float numColors) {
  int numBlades = (int) (num * 10) + 1;
  float off = 0;
  numColors = numColors * 10;
  for (int i=0; i<NUM_ROWS; ++i) {
    rows[i]->setPulse(progress, numBlades, true, color, (int)numColors, colorOffset, brightness, 2 + width * 98);
  }
}


// validate these

void rainbowRotate(float progress, float rainbowFunc, float p2, float p3, float p4, float p5) {
  for (int i=0; i<NUM_PIXELS; ++i) {
    float x = progress + i / (float) NUM_PIXELS;
    strip.setPixel(i + BASE_OFFSET, rainbowFunc > .5 ? rainbow2(x) : rainbow(x), brightness);
  }
}

void fillAllRows(float progress, float p1, float p2, float baseColor, float colorOffset, float p5) {
  for (int i=0; i<NUM_ROWS; ++i) {
    rows[i]->setAll(
      -2 * abs(progress - .5) + 1,
      i % 2 == 0 ? (progress > .5 ? true : false) : (progress > .5 ? false : true),
      rainbow(baseColor + i * colorOffset),
      brightness);
  }
}
