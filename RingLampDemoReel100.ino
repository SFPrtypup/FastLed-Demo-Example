#include <FastLED.h>
#include <Adafruit_NeoPixel.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few
// of the kinds of animation patterns you can quickly and easily
// compose using FastLED.
//
// This example also shows one easy way to define multiple
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014
//
// ********************
// Changes made to Matrix = Color from red to green
// Changes made to Matrix = Reversed the direcion
// Changes made to Sinelon = shortened the fading tail by 8
// Changes made to NextPattern = 40ms in void loop()
//
// -Stephen Johnson, April 2021

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    6
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define NUM_LEDS    90
CRGB leds[NUM_LEDS];
int pos = 0

//int buttonPin = 8;    // momentary push button on pin 0
//int oldButtonVal = 0;


#define BRIGHTNESS         120
#define FRAMES_PER_SECOND  60


#define GRAVITY           -9.81              // Downward (negative) acceleration of gravity in m/s^2
#define h0                1                  // Starting height, in meters, of the ball (strip length)
#define NUM_BALLS         4                // Number of bouncing balls you want (recommend < 7, but 20 is fun in its own way)
#define SPEED .20       // Amount to increment RGB color by each cycle
#define BG 0



//FOR SYLON ETC
;uint8_t thisbeat =  23;
uint8_t thatbeat =  28;
uint8_t thisfade =   3;                                     // How quickly does it fade? Lower = slower fade rate.
uint8_t thissat = 255;                                     // The saturation, where 255 = brilliant colours.
uint8_t thisbri = 255;



// matrix config
int thisdelay = 10;
int thisdelays = 50;                                           // A delay value for the sequence(s)
int thishues = 25;
int thissats = 255;
int thisdirs = 1;
int bgclr = 0;
int bgbri = 0; 
bool huerots = 0;
int peakcount = 0;



// Modes
enum
{
} MODE;
bool reverse = true;
int BRIGHTNESS_MAX = 100;                                                                         //80
int brightness = 100;                                                                             //20

// cycle variables
int CYCLE_MIN_MILLIS = 2;
int CYCLE_MAX_MILLIS = 1000;
int cycleMillis = 20;
bool paused = false;
long lastTime = 0;
bool boring = true;

float
  greenOffset = 30,
  blueOffset = 150;

bool gReverseDirection = false;


//background color
//uint32_t currentBg = random(256);
uint32_t currentBg = 0;
uint32_t nextBg = currentBg;
int          myhue =   100;




void setup() {                                                                                         // void setup ********
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);



  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

 // initialize the BUTTON pin as an input
//    pinMode(buttonPin, INPUT);
//    digitalWrite(buttonPin, HIGH);  // button pin is HIGH, so it drops to 0 if pressed




}




// List of patterns to cycle through.  Each is defined as a separate function below.                      patterns ************
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbowWithGlitter, sinelon, juggle, confetti, matrix, bpm, sylon, juggle, joggle, matrix };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current                             index **************
uint8_t gHue = 0; // rotating "base color" used by many of the patterns




void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter)
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 5);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 8);
  //int pos = beatsin16( 13, 0, NUM_LEDS-1 );                                                //changed
  pos++; if(pos>=NUM_LEDS) pos=0;
  leds[pos] += CHSV( gHue, 255, 192);
FastLED.show();

}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 24);                                                //10
  byte dothue = 0;
  for( int i = 0; i < 6; i++) {
    leds[beatsin16( i+5, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}



void sylon() {
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, thisfade);
  int pos1 = beatsin16(thisbeat,0,NUM_LEDS);
  int pos2 = beatsin16(thatbeat,0,NUM_LEDS);
    leds[(pos1+pos2)/3] += CHSV( myhue++/64, thissat, thisbri);
}
FastLED.show();
}


void joggle() {       // Several colored dots, weaving in and out of sync with each other

  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 24);                                                       //20
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
FastLED.show();


}


void blur() {

  uint8_t blurAmount = dim8_raw( beatsin8(3,64, 192) );       // A sinewave at 3 Hz with values ranging from 64 to 192.
  blur1d( leds, NUM_LEDS, blurAmount);                        // Apply some blurring to whatever's already on the strip, which will eventually go black.

  uint8_t  i = beatsin8(  9, 0, NUM_LEDS);
  uint8_t  j = beatsin8( 7, 0, NUM_LEDS);
  uint8_t  k = beatsin8(  5, 0, NUM_LEDS);

  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis();
  leds[(i+j)/2] = CHSV( ms / 49, 200, 255);
  leds[(j+k)/2] = CHSV( ms / 61, 200, 255);
  leds[(k+i)/2] = CHSV( ms / 93, 200, 255);
  leds[(k+i+j)/3] = CHSV( ms / 13, 200, 255);

  brightness = 100;
  FastLED.show();
  }


void matrix() {                                               // One line matrix

  if (huerots) thishues=thishues+5;

  if (random16(90) > 80) {
    if (thisdirs == 0) leds[0] = CHSV(thishues, thissats, 255); else leds[NUM_LEDS-1] = CHSV(thishues, thissats, 255);
 }
  else {
    if (thisdirs == 0) leds[0] = CHSV(bgclr, thissat, bgbri); else leds[NUM_LEDS-1] = CHSV(bgclr, thissat, bgbri);}

  if (thisdirs == 0) {
    for (int i = NUM_LEDS-1; i >0 ; i-- ) leds[i] = leds[i-1];
  } else {
    for (int i = 0; i < NUM_LEDS ; i++ ) leds[i] = leds[i+1];
    FastLED.show();
  }
} // matrix()



void loop() {                                                                                       // void loop **********


  // Call the current pattern function once, updating the 'leds' array                              array pattern *********
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();


  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 40 ) { nextPattern(); } // change patterns periodically                            change pattern *********
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))                                                   // ARRAY ************




void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end                                nextPattern *********
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}
