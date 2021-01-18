#include <Row.h>
#include <Pattern.h>
#include <Loop.h>

const uint8_t NUM_PIXELS = 252; //81
const uint8_t NUM_ROWS = 6; // 3
const uint8_t BASE_OFFSET = 0; // 0
const uint8_t ROW_SIZES[] = { 12, 24, 36, 48, 60, 72 }; //  { 15, 27, 39 }
const uint8_t NUM_PATTERNS = 5;
const uint8_t NUM_BUTTONS = 3;
const uint8_t BUTTON_PINS[] = { 42, 43, 44 };
const uint8_t NUM_POTS = 8;
const uint8_t POT_PINS[] = { 4, 5, 6, 7, 0, 1, 2, 3 };

Row* rows[NUM_ROWS];
Pattern* patterns[NUM_PATTERNS];
Loop sequence;
int currSeq = 0;

bool lastButtonStates[] = { HIGH, HIGH };

const int PIXEL_PIN = 6;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// colors
// 0 = green, .1 = light green, .2 = yellow, .3 = orange, .33 = red, .4 = magenta,
// .5 = purple, .6 = light blue, .7 = blue, .8 = aqua blue, .9 = cyan
const int NUM_SEQUENCES = 52;
const float SEQUENCES[][15] = {
  // radiate                 numRows     brightAmp   baseColor   colorOffset
  //          from one row to full rows, blue to yellow
  {  0,    24,   .17, .24,   0.0, .90,   0.0, 0.0,   .70, .20,   0.0, .14,   1.0, 1.0 },
  //          adding color offset and speeding up, mostly yellow
  {  0,    15,   .24, .26,   1.0, 1.0,   0.0, 0.0,   .20, .30,   .14, .20,   1.0, 1.0 },
  //          fast cycling through colors
  {  0,    22,   .26, .28,   1.0, 1.0,   0.0, 0.0,   .30, .70,   .20, .25,   1.0, 1.0 },
  //          more cycling, more offset - ends on green with orange
  {  0,    22,   .28, .30,   1.0, 1.0,   0.0, 0.0,   .70, .90,   .25, .45,   1.0, 1.0 },
  //          cycling to blue/purple flashing
  {  0,    22,   .30, .28,   1.0, 1.0,   0.0, 0.0,   .90, .57,   .45, .20,   1.0, 1.0 },
  //          going to full light purple, starting wave
  {  0,    18,   .28, .26,   1.0, 1.0,   0.0, .08,   .57, .51,   .20, .15,   1.0, 1.0 },
  //          getting into wave, into deeper pink
  {  0,    25,   .26, .24,   1.0, 1.0,   .08, .50,   .51, .39,   .15, 0.0,   1.0, 1.0 },
  //          more wave, slower, through red to yellow
  {  0,    25,   .24, .20,   1.0, 1.0,   .50, .90,   .39, .15,   0.0, 0.0,   1.0, 1.0 },
  //          slow down, red
  {  0,    25,   .20, .10,   1.0, 1.0,   .90, .96,   .15, .36,   0.0, 0.0,   1.0, 1.0 },
  //          slow way down, max wave, cycle to blue
  {  0,    14,   .10, .06,   1.0, 1.0,   .96, 1.0,   .36, .80,   0.0, 0.0,   1.0, 1.0 },
  //          speeding back up, less wave, to light green, starting offset
  {  0,    20,   .06, .20,   1.0, 1.0,   1.0, .68,   .80, .92,   0.0, .10,   1.0, 1.0 },
  //          more speed, slightly less wave, cycle to reddish yellow, more offset
  {  0,    25,   .20, .24,   1.0, 1.0,   .68, .58,   .92, .29,   .10, .33,   1.0, 1.0 },
  //          speed up, get rid of wave, cycle color
  {  0,    16,   .24, .28,   1.0, 1.0,   .58, 0.0,   .29, .74,   .33, .33,   1.0, 1.0 },
  //          slow down a bit, get to dual color offset
  {  0,    20,   .28, .20,   1.0, 1.0,   0.0, 0.0,   .74, .74,   .33, 1.0,   1.0, 1.0 },
  //          cycle through colors
  {  0,    4,   .20, .17,   1.0, 1.0,   0.0, 0.0,   .74, .43,   1.0, 1.0,   1.0, 1.0 },
  
  // chaser      speed       numChasers  width       color       colorOffset numColors
  //          transition to chaser - to rainbow
  {  1,    4,    .29, .24,   .20, .20,   1.0, .60,   .33, .20,   1.0, .92,   .33, .36 },
  //          slow down, shorten widths, 
  {  1,    2,    .24, .20,   .18, .16,   .55, .47,   .20, 0.0,   .92, .92,   .36, .80 },
  {  1,    4,    .20, .19,   .16, .16,   .47, .33,   0.0, .10,   .92, 4.3,   .80, 1.0 },
  {  1,    2,    .17, .16,   .17, .17,   .33, .30,   .10, .20,   1.1, 3.2,   1.0, 1.0 },
  {  1,    4,    .16, .10,   .17, .17,   .30, .21,   .20, .30,   3.2, .12,   1.0, 1.0 },
  {  1,    3,    .10, .07,   .17, .17,   .17, .17,   .30, .15,   .12, .30,   1.0, 1.0 },
  {  1,    4,    .07, .30,   .17, .16,   .17, .17,   .15, .36,   .30, 0.0,   1.0, 1.0 },
 
  {  1,    4,    .30, .34,   .14, .12,   .17, .17,   .36, .68,   0.0, .15,   1.0, 1.0 },
  {  1,    4,    .34, .30,   .12, .12,   .17, .25,   .68, .31,   .15, .80,   1.0, 1.0 },
  
  {  1,    6,    .30, .70,   .12, .12,   .25, .42,   .31, .56,   .80, .20,   1.0, 1.0 },
  {  1,    1,    .70, 0.0,   .12, .12,   .42, .30,   .56, .60,   .20, .60,   1.0, 1.0 },
  {  1,    3,    0.0, 0.0,   .08, .08,   .30, .30,   .60, .96,   .60, .80,   1.0, 1.0 },
  {  1,    4,    0.0, .40,   .12, .12,   .30, .58,   .96, .29,   .80, .50,   1.0, 1.0 },
 
  {  1,    1,    .40, .48,   .12, .20,   .58, .88,   .29, .23,   .50, .96,   1.0, 1.0 },
  {  1,    4,   .47, .39,   .20, .40,   .88, 1.0,   .29, .23,   .96, 1.0,   1.0, 1.0 },
  {  1,    1,    .39, .34,   .40, .20,   1.0, .20,   .23, .23,   .93, .92,   1.0, 1.0 },
  {  1,    4,    .34, .30,   .20, .20,   .20, .14,   .23, .26,   .92, 1.0,   1.0, 1.0 },
  {  1,    4,   .30, .27,   .19, .19,   .14, .14,   .26, .75,   1.0, 1.0,   1.0, 1.0 },
  {  1,    1,    .27, .05,   .18, .18,   .14, .14,   .75, .43,   1.0, .96,   1.0, 1.0 },
  {  1,    2,    0.0, 0.0,   .35, .35,   .11, .11,   .43, .05,   .20, .10,   1.0, 1.0 },
  
  // spinner                 numBlades   bladeWidth  baseColor   colorOffset
  {  2,    6,    0.0, .05,   1.0, .68,   .01, .03,   .05, .33,   .40, .80,   1.0, 1.0 },
  {  2,    7,    .05, .10,   .68, .35,   .03, .07,   .33, .54,   .80, .20,   1.0, 1.0 },
  {  2,    10,   .10, .13,   .35, .15,   .07, .33,   .54, .80,   .20, .50,   1.0, 1.0 },
  {  2,    2,    .13, .20,   .15, .15,   .33, .54,   .80, .58,   .50, .87,   1.0, 1.0 },
  {  2,    9,    .20, .28,   .15, .15,   .54, .45,   .58, .05,   .87, .99,   1.0, 1.0 },
  {  2,    10,   .28, .32,   .35, .35,   .45, .27,   .87, .74,   .99, .30,   1.0, 1.0 },
  {  2,    12,   .32, 0.0,   .37, .60,   .28, .10,   .74, .54,   .30, .10,   1.0, 1.0 },
  {  2,    10,   0.0, 0.0,   1.0, 1.0,   .06, .05,   .54, .32,   .10, 0.0,   1.0, 1.0 },
  {  2,    10,   0.0, .33,   1.0, 1.0,   .05, .12,   .32, .38,   0.0, .46,   1.0, 1.0 },
  {  2,    6,    .33, .40,   1.0, .53,   .12, .22,   .38, .07,   .46, 1.0,   1.0, 1.0 },
  {  2,    10,   .40, .46,   .53, .53,   .22, .15,   .07, .54,   1.0, 1.0,   1.0, 1.0 },
  {  2,    3,    .45, .29,   .53, .53,   .15, .04,   .54, .78,   1.0, 1.0,   1.0, 1.0 },
  {  2,    12,   .29, .27,   .63, .63,   .04, .04,   .78, .32,   1.0, 1.0,   1.0, 1.0 },
  {  2,    4,    .27, .18,   .63, 1.0,   .04, 0.0,   .32, .63,   1.0, 0.0,   1.0, 1.0 },

// new good ones to integrate
// crazy green radiate
  {  0,    30,   .50, .67,   0.0, 0.0,   1.0, 1.0,   .09, .09,   0.0, 0.0,   1.0, 1.0 },
// rainbow blades
  {  2,    30,   1.0, 1.0,   1.0, 1.0,   .03, .03,   .76, .76,   0.0, 1.3,   1.0, 1.0 },
  {  2,    30,   1.0, .10,   1.0, 1.0,   .03, .03,   .76, .76,   1.3, 1.3,   1.0, 1.0 },


  //       iter  speed       p1   .      p2   .      p3   .      p4   .      p5
  //          

// end of run signal
{  1,     1,   .90, 1.0,   0.0, 0.0,   0.0, 0.0,   0.0, 1.0,   0.0, 0.0,   0.0, 0.0 }
};


float patternProgress = 0.0;
int currentPattern = 0;
uint8_t brightness = 60;

uint8_t mode = 1;
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
  
  patterns[0] = new Pattern(.3, 4.5, radiate);
  patterns[1] = new Pattern(.05, .4, chaser);
  patterns[2] = new Pattern(.3, 1.8, spinner);
  patterns[3] = new Pattern(.1, 3, rainbowRotate);
  patterns[4] = new Pattern(.4, 1, fillAllRows);

  startSequence();
  
  strip.show();
}

void loop() {
  elapsedTime = millis() - currentTime;
  currentTime = millis();
  strip.clear();
  
  bool val;
  for (int i=0; i<NUM_BUTTONS; ++i) {
    val = digitalRead(BUTTON_PINS[i]);
    if (val == HIGH && lastButtonStates[i] == LOW) {
      if (i == 0)
        currentPattern = (currentPattern + 1) % NUM_PATTERNS;
      if (i == 1) {
        mode = (mode + 1) % 3;
        if (mode == 1) {
          startSequence();
        }
      }
    }
    lastButtonStates[i] = val;
  }
  
  // live mode
  if (mode == 0) {
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
    
    brightness = (int)(analogRead(POT_PINS[3]) / 1024.0 * 255);
    patternProgress = patterns[currentPattern]->tick(patternProgress, elapsedTime,
      analogRead(POT_PINS[0]) / 1024.0,
      analogRead(POT_PINS[1]) / 1024.0,
      analogRead(POT_PINS[2]) / 1024.0,
      analogRead(POT_PINS[5]) / 1024.0,
      analogRead(POT_PINS[6]) / 1024.0,
      analogRead(POT_PINS[7]) / 1024.0
    );
    patternProgress = patternProgress - (int) patternProgress;
  }
  // program mode
  else if (mode == 1) {
    bool finished = sequence.tick(elapsedTime);
    if (finished) {
      currSeq = (currSeq + 1) % NUM_SEQUENCES;
      Serial.println(currSeq);
      startSequence();
    }
  }

  strip.show();
}

