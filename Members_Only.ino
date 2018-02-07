//----------------------------------------------------
#include "FastLED.h"
//----------------------------------------------------
#define DEBUG
#if defined(DEBUG)
  #define DEBUG_PRINT(str) Serial.print(str)
  #define DEBUG_PRINT_LN(str) Serial.println(str)
  #define DEBUG_PRINT_DEC(str,dec) Serial.println(str,dec)
#else
  #define DEBUG_PRINT(str)
  #define DEBUG_PRINT_LN(str)
  #define DEBUG_PRINT_DEC(str,dec)
#endif
//----------------------------------------------------
#define NUM_LEDS 50
#define DATA_PIN 6
//----------------------------------------------------
CRGB leds[NUM_LEDS];
//----------------------------------------------------
CRGBPalette16 gradPal;

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
    0,   255,128,  0,
  128,   255,  0,100,
  200,   255,  0,255,
  255,   255,128,  0 };


int gradPos = 0;
float brightness = 0;

void setup() {
  #if defined(DEBUG)
    Serial.begin(115200);
    while (!Serial) { delay(1);}
    DEBUG_PRINT_LN(F("DEBUG MODE"));
  #endif

  setupBLE();
  
  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
  blackout(0);
  setupGradientMode();
  gradPal = heatmap_gp;
  
}

void showColor(int delayTime,int red, int green, int blue){
  delay(delayTime); 
  for(int i = 0; i < NUM_LEDS; i++) { 
    leds[i] = CRGB(red,green,blue);
  }
  FastLED.show();
}

void blackout(int delayTime) {
  delay(delayTime); 
  for(int i = 0; i < NUM_LEDS; i++) { 
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void loop() { 
  BLELoop();
  FastLED.setBrightness(brightness);
  if(brightness < 255){
    brightness += 0.05;
  }
  DEBUG_PRINT_LN(brightness);
  fill_solid(leds,NUM_LEDS,ColorFromPalette( gradPal, gradPos));
  gradPos++;
  FastLED.show();
  if(gradPos > 255){
    gradPos = 0;
  }
  
}


