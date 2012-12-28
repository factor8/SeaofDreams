#include "SPI.h"
#include "Adafruit_WS2801.h"
#include <Easing.h>
#include <TrueRandom.h>

/*****************************************************************************
Example sketch for driving Adafruit WS2801 pixels!


  Designed specifically to work with the Adafruit RGB Pixels!
  12mm Bullet shape ----> https://www.adafruit.com/products/322
  12mm Flat shape   ----> https://www.adafruit.com/products/738
  36mm Square shape ----> https://www.adafruit.com/products/683

  These pixels use SPI to transmit the color data, and have built in
  high speed PWM drivers for 24 bit color per pixel
  2 pins are required to interface

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution

*****************************************************************************/

// Choose which 2 pins you will use for output.
// Can be any valid output pins.
// The colors of the wires may be totally different so
// BE SURE TO CHECK YOUR PIXELS TO SEE WHICH WIRES TO USE!
int dataPin  = 8;    // Yellow wire on Adafruit Pixels
int clockPin = 10;    // Green wire on Adafruit Pixels

// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(104, dataPin, clockPin);

// Optional: leave off pin numbers to use hardware SPI
// (pinout is then specific to each board and can't be changed)
//Adafruit_WS2801 strip = Adafruit_WS2801(25);

// For 36mm LED pixels: these pixels internally represent color in a
// different format.  Either of the above constructors can accept an
// optional extra parameter: WS2801_RGB is 'conventional' RGB order
// WS2801_GRB is the GRB order required by the 36mm pixels.  Other
// than this parameter, your code does not need to do anything different;
// the library will handle the format change.  Examples:
//Adafruit_WS2801 strip = Adafruit_WS2801(25, dataPin, clockPin, WS2801_GRB);
//Adafruit_WS2801 strip = Adafruit_WS2801(25, WS2801_GRB);

void setup() {
    
  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();
}

void loop() {
  // Some example procedures showing how to display to the pixels
//  sparkle(Color(255,255,255), 10, 100);
//  RandomSparkle();
//  colorWipe(Color(TrueRandom.random(0,255), TrueRandom.random(0,255), TrueRandom.random(0,255)), 1);
//rainbow(1);
  int go = 0;
  colorWipe(Color(255,0,0), 10);
  colorWipe(Color(255,255,0), 10);
  colorWipe(Color(255,255,255), 10);
  for(go=0;go<32;go++) {  ColorJump(200);   }
  for(go=0;go<32;go++) { SinglePixel(TrueRandom.random(1, 30)); }
  for(go=0;go<32;go++) { SparkleRandom(); }
  for(go=0;go<8;go++) { ColorJump(100); }
  SparkleFaster(3000);
  SparkleSlower(3000);
}


void BlackToBright(uint8_t r, uint8_t g, uint8_t b, uint8_t DELAY){
  int i, value;
  for(i=0;i<strip.numPixels();i++) {
      strip.setPixelColor(i, Color(0,0,0));
   } 
   strip.show();
   for(value=0;value<101;i++) {
     r = r * (value/100);
     g = g * (value/100);
     b = b * (value/100);
     for(i=0;i<strip.numPixels();i++) {
        strip.setPixelColor(i, Color(r,g,b));
     } 
     strip.show(); 
     delay(DELAY);
   }
}

class Sparkle {
   uint32_t color, density, del;
   public:
     Sparkle (uint32_t, uint8_t, uint8_t)
}

Sparkle::Sparkle(){
  
  int Total=strip.numPixels();
  int r; 
  int i;
  int PIXEL; 

  for(i=0; i < strip.numPixels(); i++ ) {
    strip.setPixelColor(i, Color(0,0,0));
  }
  strip.show();
  
  for( r=0; r < 10; r++ )  {
   PIXEL = TrueRandom.random(1,Total);
   strip.setPixelColor(PIXEL, Color(255,255,255));
  }
  strip.show();
  
  delay(DELAY);
 
}

Sparkle::Sparkle(uint32_t COLOR, uint8_t DENSITY, uint8_t DELAY){
  
  int Total=strip.numPixels();
  int r; 
  int i;
  int PIXEL; 

  for(i=0; i < strip.numPixels(); i++ ) {
    strip.setPixelColor(i, Color(0,0,0));
  }
  strip.show();
  
  for( r=0; r < DENSITY; r++ )  {
   PIXEL = TrueRandom.random(1,Total);
   strip.setPixelColor(PIXEL, COLOR);
  }
  
  strip.show();
  
  delay(DELAY);
 
}

//void SparkleFaste(uint8_t duration){
//  int go;
//  for(go=0;go<100;go++) {
//    SinglePixel( Easing::easeInOutCubic(go, 0, 140, duration) );
//  }
//}
//
//void Sparkle::Slower(uint8_t duration){
//  int go;
//  for(go=0;go<100;go++) {
//    SinglePixel( Easing::easeInOutCubic(go, 140, 0, duration) );
//  }
//}

void SparkleFaster(uint8_t duration){
  int go;
  for(go=0;go<100;go++) {
    SinglePixel( Easing::easeInOutCubic(pos, 0, 140, duration) );
  }
}

void SparkleSlower(uint8_t duration){
  int go;
  for(go=0;go<100;go++) {
    SinglePixel( Easing::easeInOutCubic(pos, 140, 0, duration) );
  }
}


void SparkleRandom(){
  int COLOR = Color(TrueRandom.random(0,255), TrueRandom.random(0,255), TrueRandom.random(0,255));
  int DENSITY = TrueRandom.random(1,30);
  int DELAY = TrueRandom.random(25,160);
  
  sparkle(COLOR, DENSITY, DELAY);  
}

void ColorJump(uint8_t DELAY){
  int i;
  uint32_t COLOR;
  COLOR = RandomColor();
  for(i=0;i<strip.numPixels();i++) { strip.setPixelColor(i, COLOR); }
  strip.show();
  delay(DELAY/2);
  for(i=0;i<strip.numPixels();i++) { strip.setPixelColor(i, Color(0,0,0)); }
  strip.show();
  delay(DELAY/2);
}


void SinglePixel(uint8_t DELAY){
 sparkle(Color(255,255,255), 1, DELAY);
}

void CrazyPixel(uint32_t COLOR){
  sparkle(COLOR, 4, 1);
}

void sparkle(uint32_t COLOR, uint8_t DENSITY, uint8_t DELAY){
  
    int Total=strip.numPixels();
    int r; 
    int i;
    int PIXEL; 

  //generating several intergers
  
  for(i=0; i < strip.numPixels(); i++ ) {
    strip.setPixelColor(i, Color(0,0,0));
  }
  strip.show();
  
  for( r=0; r < DENSITY; r++ )  {
   PIXEL = TrueRandom.random(1,Total);
   strip.setPixelColor(PIXEL, COLOR);
  }
  strip.show();
  
  delay(DELAY);
 //
 
}


void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

uint32_t RandomColor(){
   return Color(TrueRandom.random(0,255), TrueRandom.random(0,255), TrueRandom.random(0,255));
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
