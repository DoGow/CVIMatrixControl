#include "FastLED.h"

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR 
#endif

// How many leds in your strip?
#define NUM_LEDS 88
#define DATA_PIN 12
#define BRIGHTNESS  10
//#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];


char LetterToColor(char buffer);

const int ledPin = 12; // the pin that the LED is attached to
int incomingByte;      // a variable to read incoming serial data into
static int pos;
static int color;

int  pixelPerChar = 6;
int  maxDisplacement;

//////////////////////////////////////////////
/* those are the values for the temp input*/
////////////////////////////////////////////

char buffer [7];
char string[20];
char test[6] = {'H', 'E', 'L', 'L', 'O', '\0'};

//////////////////////////////////////////////////////*
/*  C = Clear all
 *  S = Singel Led Control
 *  M = Matirx
 *  T = Text
*/ 
///////////////////////////////////////////////
char Mode;


static char rColorName[6];


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(7, 7, DATA_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };


int x    = matrix.width();
int pass = 0;
 
void setup() {
   FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
   //FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
   FastLED.setBrightness(BRIGHTNESS );
     matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(10);
  matrix.setTextColor(colors[0]);
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}


void loop()
{
  
}

void serialEvent() {

// see if there's incoming serial data:
  //while (Serial.available())
  Mode = Serial.read();
  delay(10);
  switch (Mode) 
  {
    case 'S':
    //Singel LED
        //if (Serial.available() >= 7) 
       // {
             while (Serial.available() < 4)
                {
                  //wait for data
                }
              Serial.readBytes(buffer, 4);
              leds[buffer[3]] = CRGB(buffer[0], buffer[1], buffer[2]);   ///RGB
               FastLED.show();
              delay(500);
              leds[buffer[6]] = CRGB::Black;
              FastLED.show();
              delay(500);
              Serial.flush();
              break;
        //}
     case 'M':
     //Send Matrix
          for(int k=0;k<NUM_LEDS;k++)
           {
                while (Serial.available() < 4)
                {
                  //wait for data
                }
                   Serial.readBytes(buffer, 4);
                  leds[buffer[3]] = CRGB(buffer[0], buffer[1], buffer[2]);   ///RGB
                  FastLED.show();
                  //delay(50);
          
             }
              Serial.flush();
              break;
       case 'T':
       //Send Text    
            int k;    
            for (int i=0; i<20; i++) 
            {
               string[i] = Serial.read();
               if (string[i]==NULL) break;
            }
      
            maxDisplacement = strlen(string) * pixelPerChar + matrix.width();
            
            for(k=260;k>0;k--)
            {
                matrix.fillScreen(0);
                matrix.setCursor(x, 0);
                matrix.print((string));
                if(--x < -maxDisplacement) {
                x = matrix.width();
                if(++pass >= 3) pass = 0;
                matrix.setTextColor(colors[pass]);
            }
            matrix.show();
            delay(100);
            }
              Serial.flush();
              break;
              

        case 'C':
            //clear mode
            for (int i = 0; i < NUM_LEDS; i++)
            {
              leds[i] = CRGB::Black;
            }
      
            FastLED.show();
            delay(50);
            //Serial.flush();
            break;
      }
}


