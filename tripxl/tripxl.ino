#include <Row.h>
#include <Pattern.h>
#include <Loop.h>

const uint8_t NUM_PIXELS = 81; // 252
const uint8_t NUM_ROWS = 3; // 6
const uint8_t BASE_OFFSET = 0; // 0
const uint8_t ROW_SIZES[] = { 15, 27, 39 }; // { 12, 24, 36, 48, 60, 72 }
const uint8_t NUM_PATTERNS = 6;

uint8_t colorMode = 0;
uint32_t mainColor = 0x336699;
uint32_t colorList[] = { 0x3300cc, 0x66cc00, 0xcc6600, 0x990066, 0xcc0033, 0x00cc66, 0x11ff11 };
const uint8_t COLOR_LIST_LENGTH = 7;

// connections: 
// green data wire from pixel         -> pin 6 on arduino
// white wire next to green on pixel  -> ground on psu
// pink wire from pixel               -> power on psu
// white wire next to pink from pixel -> neutral on psu (in testing it worked on -V, not the actual neutral from the wall)
// VIN and ground on arduino          -> power and ground on psu

const int PIXEL_PIN = 6;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

Row* rows[NUM_ROWS];
Pattern* patterns[NUM_PATTERNS];
Loop animation;
bool animationIsPlaying = true;

long currentTime = 0;
long elapsedTime = 0;

void setup() {
  Serial.begin(9600);
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
  
  strip.show();

//  Serial.println("get");
//  delay(200);
//  Serial.println("get");
  animation = Loop(patterns[5],  3,   1,   .5,   .5,    6,   6, 1, 9, 0, 0, 0, 0, 0, 0);
}

void loop() {
  if (Serial.available() > 0) {
    animationIsPlaying = true;
    animation.pattern = patterns[readI()];
    animation.iterations = readI();
    animation.doReverse = readB();
    animation.speedStart = readF();
    animation.speedEnd = readF();
    animation.p1s = readF();
    animation.p1e = readF();
    animation.p2s = readF();
    animation.p2e = readF();
    animation.p3s = readF();
    animation.p3e = readF();
    animation.p4s = readF();
    animation.p4e = readF();
    animation.p5s = readF();
    animation.p5e = readF();

    if (Serial.read() == '\n') {
      Serial.println("ya done good");
    } else {
      Serial.println("ya fucked up");
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

