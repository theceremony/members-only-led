#include <FastLED.h>


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

#define NUM_LEDS 50
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  #if defined(DEBUG)
    Serial.begin(115200);
    while (!Serial) { delay(1);}
    DEBUG_PRINT_LN(F("DEBUG MODE"));
  #endif

  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
}

void loop() { 
//        for(int i = 0; i < NUM_LEDS; i++) { 
//          leds[i] = CRGB::DeepSkyBlue;
//        }
//        FastLED.show();
//        delay(1000); 

        for(int i = 0; i < NUM_LEDS; i++) { 
          leds[i] = CRGB::Black;
        }
        FastLED.show();
        delay(1000); 
    }
