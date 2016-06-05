// number of rows on, rainbow color [0-1], brightness difference, nothing, nothing
void radiate(float progress, float p1, float p2, float p3, float p4, float p5) {
  int row = (int) (progress * NUM_ROWS);
  for (int i=0; i < (int) p1; ++i) {
      rows[(row + i) % NUM_ROWS]->setAll(1, false, rainbow(round(p2 * 255)), 80 + (i - ((int) p1 - 1)/2) * (int) p3);
  }
}

// number of snakes, width of snakes, direction, nothing, nothing
void snake(float progress, float num, float width, float dir, float p4, float p5) {
  for (int j=0; j<num; ++j)
    for (int i=0; i<width; ++i) {
      strip.setPixelColor(
        BASE_OFFSET + (round(((int) dir == 0 ? progress : (1 - progress)) * NUM_PIXELS) + i + round(j * NUM_PIXELS / (float) num)) % NUM_PIXELS,
        colorList[j % COLOR_LIST_LENGTH]
      );
    }
}

// number of pulses, width of pulses, percent or absolute width, nothing, nothing
void spinner(float progress, float num, float width, float mode, float p4, float p5) {
  for (int i=0; i<NUM_ROWS; ++i) {
    if ((int) mode == 0)
      // set width using number of lights
      rows[i]->setPulse(progress, (int) num, (int) width, true, colorList, COLOR_LIST_LENGTH, 80);
    else
      // set width using percentage
      rows[i]->setPulse(progress, (int) num, true, colorList, COLOR_LIST_LENGTH, 80, width);
      
  }
}





// validate these

void rainbowCycle(float progress, float p1, float p2, float p3, float p4, float p5) {
  for (int i=0; i<NUM_ROWS; i++) {
    rows[i]->setAll(1, false, rainbow((round(p1 * 255 + p2 * i)) % 255), 128);
  }
}

void rainbowRotate(float progress, float p1, float p2, float p3, float p4, float p5) {
  for (int i=0; i<NUM_PIXELS; ++i) {
    strip.setPixelColor(i + BASE_OFFSET, rainbow((round(progress * 255 + i / (float) NUM_PIXELS * 255.0)) % 255));
  }
}

void fillAllRows(float progress, float p1, float p2, float p3, float p4, float p5) {
  for (int i=0; i<NUM_ROWS; ++i) {
      rows[i]->setAll(
        -2 * abs(progress - .5) + 1,
        i % 2 == 0 ? (progress > .5 ? true : false) : (progress > .5 ? false : true),
        rainbow(round(progress * 60 + 50 + i * 12)),
        80);
  }
}
