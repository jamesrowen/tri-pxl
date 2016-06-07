#include <Row.h>
#include <Pattern.h>
#include <Loop.h>

const uint8_t NUM_PIXELS = 81; // 252
const uint8_t NUM_ROWS = 3; // 6
const uint8_t BASE_OFFSET = 0; // 0
const uint8_t ROW_SIZES[] = { 15, 27, 39 }; // { 12, 24, 36, 48, 60, 72 }
const uint8_t NUM_PATTERNS = 6;

Row* rows[NUM_ROWS];
Pattern* patterns[NUM_PATTERNS];
Loop animation;
bool animationIsPlaying = true;

uint8_t colorMode = 0;

const int PIXEL_PIN = 6;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

long currentTime = 0;
long elapsedTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("ack");
  strip.begin();
  strip.show();

  int offset = 0;
  for (int i=0; i<NUM_ROWS; i++) {
    rows[i] = new Row(strip, BASE_OFFSET + offset, ROW_SIZES[i]);
    offset += ROW_SIZES[i];
  }
  
  patterns[0] = new Pattern(.16, 2.6, rainbowCycle);
  patterns[1] = new Pattern(.1, .5, snake);
  patterns[2] = new Pattern(.1, 3, rainbowRotate);
  patterns[3] = new Pattern(.1, 4, radiate);
  patterns[4] = new Pattern(.4, 1, fillAllRows);
  patterns[5] = new Pattern(.5, 2, spinner);

  // starting animation
  animation = Loop(patterns[2], 1, 0, .05, .05, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  
  strip.show();
}

void loop() {
  if (Serial.available() > 0) {
    int marker = Serial.parseInt();
    // process action
    if (marker < 0) {
      Serial.read();
      String action = Serial.readStringUntil(' ');
      int p1 = Serial.parseInt();
      int p2 = Serial.parseInt();
      int p3 = Serial.parseInt();
      int p4 = Serial.parseInt();
      doAction(action, p1, p2, p3, p4);
      if (Serial.read() == '\n') {
        Serial.println("action: " + action);
      } else {
        Serial.println("action FUCKED");
      }
      Serial.println("get");
    }
    // read new loop
    else {
      animationIsPlaying = true;
      animation.pattern = patterns[marker];
      animation.iterations = Serial.parseInt();
      animation.doReverse = (bool) Serial.parseInt();
      animation.speedStart = Serial.parseFloat();
      animation.speedEnd = Serial.parseFloat();
      animation.p1s = Serial.parseFloat();
      animation.p1e = Serial.parseFloat();
      animation.p2s = Serial.parseFloat();
      animation.p2e = Serial.parseFloat();
      animation.p3s = Serial.parseFloat();
      animation.p3e = Serial.parseFloat();
      animation.p4s = Serial.parseFloat();
      animation.p4e = Serial.parseFloat();
      animation.p5s = Serial.parseFloat();
      animation.p5e = Serial.parseFloat();

      // basic sanity check
      if (Serial.read() == '\n') {
        Serial.println("loop good");
      } else {
        Serial.println("loop FUCKED");
      }
    }
  }
  strip.clear();
  elapsedTime = millis() - currentTime;
  currentTime = millis();
  if (animationIsPlaying && animation.tick(elapsedTime)) {
    Serial.println("get");
    animation.start();
  }

  strip.show();
}

void doAction(String action, int p1, int p2, int p3, int p4) {
  if (action == "setColorMode") {
    colorMode = p1;
  }
}

