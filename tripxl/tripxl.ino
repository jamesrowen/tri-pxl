#include <LinkedList.h>
#include <FastLED.h>
#define DATA_PIN 6

const uint8_t NUM_LEDS = 252;
const uint8_t NUM_ROWS = 6;
const uint8_t BASE_OFFSET = 0;
const uint8_t ROW_SIZES[] = { 12, 24, 36, 48, 60, 72 };
const uint8_t ROW_STARTS[] = { 0, 12, 36, 72, 120, 180 };
const uint8_t NUM_PATTERNS = 5;
const uint8_t NUM_SEQ_PARAMS = 7;
//const uint8_t NUM_BUTTONS = 3;
//const uint8_t BUTTON_PINS[] = { 42, 43, 44 };
//const uint8_t NUM_POTS = 8;
const uint8_t POT_PINS[] = { 4, 5, 6, 7, 0, 1, 2, 3 };

CRGB leds[NUM_LEDS];
uint8_t mode = 0;
uint8_t maxBrightness = 16;

long currentTime = 0;
long elapsedTime = 0;
//bool lastButtonStates[] = { HIGH, HIGH };

// -------------------------
// Pattern
// -------------------------
struct Pattern {
  float minSpeed, maxSpeed;
  void (*update)(float progress, float p1, float p2, float p3, float p4, float p5);
  Pattern() {}
  Pattern(float _min, float _max, void(*_upd)(float, float, float, float, float, float)) : minSpeed(_min), maxSpeed(_max), update(_upd) {
  }
};
float patternPosition = 0.0;
int curPat = 0;
Pattern patterns[NUM_PATTERNS];

float tickPattern(float progress, long elapsed, float speedFactor, float p1, float p2, 
  float p3, float p4, float p5) {
  progress += elapsed / 1000.0 * ((patterns[curPat].maxSpeed - patterns[curPat].minSpeed) * speedFactor + patterns[curPat].minSpeed);
  patterns[curPat].update(progress, p1, p2, p3, p4, p5);
  return progress;
}

// -------------------------
// Sequence
// -------------------------
struct Keyframe {
  float time;
  float value;
  Keyframe() {}
  Keyframe(float t, float v) : time(t), value(v) {
  }
};

struct Sequence {
  LinkedList<Keyframe> params[NUM_SEQ_PARAMS];
  int length;
  Sequence() {}
};
Sequence seq;
int seqPosition = 0;

float tickSequence(float elapsedTime) {
  curPat = (int) getParam(0, false);
  patternPosition = tickPattern(patternPosition, elapsedTime, getParam(1), getParam(2), 
  getParam(3), getParam(4), getParam(5), getParam(6));
  if (patternPosition >= 1) {
    patternPosition = patternPosition - (int) patternPosition;
    seqPosition = (seqPosition + 1) % seq.length;
  }
}

// interpolate parameter value based on its keyframes and the sequence position
float getParam(int i, bool doLerp) {
  // if no keyframes, return a default value
  if (i >= NUM_SEQ_PARAMS || seq.params[i].size() == 0) {
    return .5;
  }
  // find the next keyframe
  int kfIndex = 0;
  while (kfIndex < seq.params[i].size() && seq.params[i].get(kfIndex).time <= seqPosition) {
    kfIndex++;
  }
  // if we're past the last keyframe
  if (kfIndex >= seq.params[i].size()) {
    return seq.params[i].get(seq.params[i].size() - 1).value;
  }
  // if we're between keyframes
  Keyframe next = seq.params[i].get(kfIndex);
  Keyframe prev = seq.params[i].get(kfIndex - 1);
  float x = (seqPosition - prev.time) / (next.time - prev.time);
  return doLerp ? lerp(prev.value, next.value, x) : prev.value;
}
float getParam(int i) {
  return getParam(i, true);
}

// -------------------------
// Row
// -------------------------
void setRow(int row, float percentage, CRGB color, int fadeAmount, bool reverse) {
  for (int i = 0; i < int(ROW_SIZES[row] * percentage); i++) {
    int index = reverse ? ROW_STARTS[row] + ROW_SIZES[row] - i - 1 : ROW_STARTS[row] + i;
    leds[index] = color;
    leds[index].fadeLightBy(fadeAmount);
  }
}


// -------------------------
// Main
// -------------------------
void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(maxBrightness);
  Serial.begin(9600);
  
  patterns[0] = Pattern(.2, 4, radiate);
  patterns[1] = Pattern(.03, .3, chaser);
  patterns[2] = Pattern(.2, 1.8, spinner);
  patterns[3] = Pattern(.1, 3, rainbowRotate);
  patterns[4] = Pattern(.4, 1, fillAllRows);

  seq.length = 3;
  seq.params[0].add(Keyframe(0, 0)); // pattern
  seq.params[1].add(Keyframe(0, 0)); // speed
  seq.params[2].add(Keyframe(0, .8)); // p1
  seq.params[3].add(Keyframe(0, .5)); // p2
  seq.params[4].add(Keyframe(0, .4)); // p3
  seq.params[5].add(Keyframe(0, .3)); // p4
  seq.params[6].add(Keyframe(0, 0)); // p5
  
  seq.params[1].add(Keyframe(4, .17)); // speed
  seq.params[2].add(Keyframe(4, 0)); // p1
  seq.params[3].add(Keyframe(4, 0)); // p2
  seq.params[4].add(Keyframe(4, .7)); // p3
  seq.params[5].add(Keyframe(4, 0)); // p4
  seq.params[6].add(Keyframe(4, 1)); // p5
  
  seq.params[1].add(Keyframe(10, .24)); // speed
  seq.params[2].add(Keyframe(10, .9)); // p1
  seq.params[4].add(Keyframe(10, .2)); // p3
  seq.params[5].add(Keyframe(10, .14)); // p4
  seq.params[1].add(Keyframe(14, .24)); // speed
  seq.params[2].add(Keyframe(14, .9)); // p1
  seq.params[4].add(Keyframe(14, .2)); // p3
  seq.params[5].add(Keyframe(14, .14)); // p4
  
  seq.params[0].add(Keyframe(15, 1)); // pattern
  seq.params[1].add(Keyframe(15, .15)); // speed
  seq.params[2].add(Keyframe(15, .08)); // p1
  seq.params[3].add(Keyframe(15, .3)); // p2
  seq.params[4].add(Keyframe(15, .4)); // p3
  seq.params[5].add(Keyframe(15, .3)); // p4
  seq.params[6].add(Keyframe(15, .5)); // p5
  
  seq.params[0].add(Keyframe(16, 2)); // pattern
  seq.params[1].add(Keyframe(16, .1)); // speed
  seq.params[2].add(Keyframe(16, .3)); // p1
  seq.params[3].add(Keyframe(16, .1)); // p2
  seq.params[4].add(Keyframe(16, .4)); // p3
  seq.params[5].add(Keyframe(16, .3)); // p4
  seq.params[6].add(Keyframe(16, 1)); // p5
  
  seq.params[0].add(Keyframe(17, 3)); // pattern
  seq.params[1].add(Keyframe(17, .15)); // speed
  
  seq.params[0].add(Keyframe(18, 4)); // pattern
  seq.params[1].add(Keyframe(18, 0)); // speed
  seq.params[2].add(Keyframe(18, .3)); // p1
  seq.params[3].add(Keyframe(18, .1)); // p2
  seq.params[4].add(Keyframe(18, .4)); // p3
  seq.params[5].add(Keyframe(18, .1)); // p4
  seq.params[6].add(Keyframe(18, .2)); // p5
  // colors
  // 0 = green, .1 = light green, .2 = yellow, .3 = orange, .33 = red, .4 = magenta,
  // .5 = purple, .6 = light blue, .7 = blue, .8 = aqua blue, .9 = cyan
}

void loop() {
  elapsedTime = millis() - currentTime;
  currentTime = millis();

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  
//  bool val;
//  for (int i=0; i<NUM_BUTTONS; ++i) {
//    val = digitalRead(BUTTON_PINS[i]);
//    if (val == HIGH && lastButtonStates[i] == LOW) {
//      if (i == 0)
//        currentPattern = (currentPattern + 1) % NUM_PATTERNS;
//      if (i == 1) {
//        mode = (mode + 1) % 3;
//        if (mode == 1) {
//          startSequence();
//        }
//      }
//    }
//    lastButtonStates[i] = val;
//  }

  if (mode == 0) {
    tickSequence(elapsedTime);
  } else {
    Serial.print(analogRead(POT_PINS[0]) / 1024.0);
    Serial.print(' ');
    Serial.print(analogRead(POT_PINS[1]) / 1024.0);
    Serial.print(' ');
    Serial.print(analogRead(POT_PINS[2]) / 1024.0);
    Serial.print(' ');
    Serial.print(analogRead(POT_PINS[3]) / 1024.0);
    Serial.print(' ');
    Serial.print(analogRead(POT_PINS[4]) / 1024.0);
    Serial.print(' ');
    Serial.print(analogRead(POT_PINS[5]) / 1024.0);
    Serial.print(' ');
    Serial.print(analogRead(POT_PINS[6]) / 1024.0);
    Serial.print(' ');
    Serial.println(analogRead(POT_PINS[7]) / 1024.0);
    
    maxBrightness = (int)(analogRead(POT_PINS[3]) / 1024.0 * 255);
    patternPosition = tickPattern(patternPosition, elapsedTime,
      analogRead(POT_PINS[0]) / 1024.0,
      analogRead(POT_PINS[1]) / 1024.0,
      analogRead(POT_PINS[2]) / 1024.0,
      analogRead(POT_PINS[5]) / 1024.0,
      analogRead(POT_PINS[6]) / 1024.0,
      analogRead(POT_PINS[7]) / 1024.0
    );
    patternPosition = patternPosition - (int) patternPosition;
  }
  FastLED.show();
}
