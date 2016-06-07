// number of rows on, rainbow color [0-1], brightness difference, nothing, nothing
void radiate(float progress, float numRows, float color, float brightDiff, float p4, float p5) {
  int row = (int) (progress * NUM_ROWS);
  for (int i=0; i < (int) numRows; ++i) {
      rows[(row + i) % NUM_ROWS]
        ->setAll(1, false, rainbow(color), 80 + (i - ((int) numRows - 1)/2) * (int) brightDiff);
  }
}

// number of snakes, width of snakes, direction, baseColor, colorOffset
void snake(float progress, float num, float width, float dir, float baseColor, float colorOffset) {
  for (int j=0; j<num; ++j)
    for (int i=0; i<width; ++i) {
      strip.setPixel(
        BASE_OFFSET + (round(((int) dir == 0 ? progress : (1 - progress)) * NUM_PIXELS) + i + round(j * NUM_PIXELS / (float) num)) % NUM_PIXELS,
        rainbow(baseColor + colorOffset * j),
        100
      );
    }
}

// number of pulses, width of pulses, percent or absolute width, nothing, nothing
void spinner(float progress, float num, float width, float widthMode, float color, float colorOffset) {
  for (int i=0; i<NUM_ROWS; ++i) {
    if ((int) widthMode == 0)
      // set width using number of lights
      rows[i]->setPulse(progress, (int) num, (int) width, true, color, 6, colorOffset, 80);
    else
      // set width using percentage
      rows[i]->setPulse(progress, (int) num, true, color, 6, colorOffset, 80, width);
      
  }
}





// validate these

void rainbowCycle(float progress, float baseColor, float colorOffsetInt, float p3, float p4, float p5) {
  for (int i=0; i<NUM_ROWS; i++) {
    rows[i]->setAll(1, false, rainbow(baseColor + colorOffsetInt * i), 80);
  }
}

void rainbowRotate(float progress, float rainbowFunc, float p2, float p3, float p4, float p5) {
  for (int i=0; i<NUM_PIXELS; ++i) {
    float x = progress + i / (float) NUM_PIXELS;
    strip.setPixel(i + BASE_OFFSET, rainbowFunc > .5 ? rainbow2(x) : rainbow(x), 100);
  }
}

void fillAllRows(float progress, float baseColor, float colorOffset, float p3, float p4, float p5) {
  for (int i=0; i<NUM_ROWS; ++i) {
      rows[i]->setAll(
        -2 * abs(progress - .5) + 1,
        i % 2 == 0 ? (progress > .5 ? true : false) : (progress > .5 ? false : true),
        rainbow(baseColor + i * colorOffset),
        80);
  }
}
