#include "SPI.h"
#include "Adafruit_WS2801.h"
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

//For spectrum analyzer shield, these three pins are used.
//You can move pinds 4 and 5, but you must cut the trace on the shield and re-route from the 2 jumpers. 
int spectrumReset=5;
int spectrumStrobe=4;
int spectrumAnalog=0;  //0 for left channel, 1 for right.

// Spectrum analyzer read values will be kept here.
int Spectrum[7];

int average = 0;

int kickThreshold = 0;
int snareThreshold = 0;

int kick = 0;
int snare = 0;

// Division sets the strand mode. 1 is regular, 2 is Mirror, 3 is Triangle (not added yet), 4 is Radial.
int division = 2;

// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(72	, dataPin, clockPin);

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
  
  Serial.begin(9600);
  

  //Setup pins to drive the spectrum analyzer. 
  pinMode(spectrumReset, OUTPUT);
  pinMode(spectrumStrobe, OUTPUT);

  //Init spectrum analyzer
  digitalWrite(spectrumStrobe,LOW);
  delay(1);
  digitalWrite(spectrumReset,HIGH);
  delay(1);
  digitalWrite(spectrumStrobe,HIGH);
  delay(1);
  digitalWrite(spectrumStrobe,LOW);
  delay(1);
  digitalWrite(spectrumReset,LOW);
  delay(5);
  // Reading the analyzer now will read the lowest frequency.

  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();
	
	// Serial.println(Color(100,0,1));
	// Serial.println(numeric_limits<uint8_t>::max());
}


void loop() {

  //  Console Output Testing
	// Serial.println(millis()/1000);
  // Serial.println(strip.getPixelColor(2));

  // Update the spectrum values.
	// readSpectrum();

	// rainbowCycle(10);
	
  // showSpectrum(10);
	// CrazyPixel(Color(0,0,45));
	colorWipe(rgba(255, 255, 255,1), 5);
	// ASCSparkle();

	// BlackToBright(0,44,0,10);

  
  // Some example procedures showing how to display to the pixels
  
//  colorWipe(Color(255, 0, 0), 50);
//  colorWipe(Color(0, 255, 0), 50);
//  colorWipe(Color(0, 0, 255), 50);
//  rainbow(1000);
//  rainbowCycle(20);
}

void showSpectrum(uint8_t wait)
{
	
  byte Band, BarSize, MaxLevel;
  static unsigned int  Divisor = 20, ChangeTimer=0;
  unsigned int works, Remainder;

  int peak = Spectrum[2]/Divisor;
	int wheel = Spectrum[1]/10;

  MaxLevel = 0; 

   for (int i=0; i < strip.numPixels() / division; i++) {
     if (i <= peak) {
 				// mirror(i, Wheel(wheel+(i*4) ),1); 
				q(i,Color(0,0,45));
   			// mirror(i, Wheel(random(0,wheel)),1); 
      } else {
        q(i, Color(0,0,0));
   		}
   			Serial.println(peak);
   }  
		
  // strip.show();
  
  
//    //If value is 0, we don't show anything on graph
//    works = Spectrum[Band]/Divisor;	//Bands are read in as 10 bit values. Scale them down to be 0 - 5
//    if(works > MaxLevel)  //Check if this value is the largest so far.
//      MaxLevel = works;                       
  

  strip.show();
  delay(wait);
}




// Read 7 band equalizer.
void readSpectrum()
{
  // Band 0 = Lowest Frequencies.

	int display_eq = 1;
	average = 0;

  byte Band;
  for(Band=0;Band <7; Band++)
  {
    Spectrum[Band] = (analogRead(spectrumAnalog) + analogRead(spectrumAnalog) ) >>1; //Read twice and take the average by dividing by 2
    digitalWrite(spectrumStrobe,HIGH);
    digitalWrite(spectrumStrobe,LOW);     
		average += Spectrum[Band];

    // Serial.print(Spectrum[Band]/80);       
    // Serial.print(" ");     
  }

	average = average/7; 	
	// Serial.print(Spectrum[1]);
	// Serial.print(" - ");
	// Serial.print(Spectrum[6]);
	// Serial.print(" - ");
	// Serial.println(average);
	
	if (Spectrum[1] > average) {
		// Serial.print("Kick - "); 
		// Serial.println(Spectrum[1]); 
		kick = 1;
	} else {kick=0;}
	if (Spectrum[6] > average) {
		// Serial.print("Snare - "); 
		// Serial.println(Spectrum[1]); 
		snare = 1;
	} else {snare=0;}



  // Serial.println(average);
}


void BlackToBright(uint8_t r, uint8_t g, uint8_t b, uint8_t DELAY){
  int i, value;
  for(i=0;i<strip.numPixels()/division;i++) {
      q(i, Color(0,0,0));
   } 
   strip.show();
   for(value=0;value<101;i++) {
     r = r * (value/100);
     g = g * (value/100);
     b = b * (value/100);
     for(i=0;i<strip.numPixels()/division;i++) {
        q(i, Color(r,g,b));
     } 
     strip.show(); 
     delay(DELAY);
   }
}

void ASCSparkle(){
  int go;
  for(go=0;go<8;go++) { SinglePixel(100); }
  for(go=0;go<8;go++) { SinglePixel(90); }
  for(go=0;go<8;go++) { SinglePixel(75); }
  for(go=0;go<8;go++) { SinglePixel(55); }
  for(go=0;go<8;go++) { SinglePixel(35); }
  for(go=0;go<8;go++) { SinglePixel(15); }
  for(go=0;go<8;go++) { SinglePixel(1); }
}

void DESCSparkle(){
  int go;
  for(go=0;go<8;go++) { SinglePixel(1); }
  for(go=0;go<8;go++) { SinglePixel(15); }
  for(go=0;go<8;go++) { SinglePixel(35); }
  for(go=0;go<8;go++) { SinglePixel(55); }
  for(go=0;go<8;go++) { SinglePixel(75); }
  for(go=0;go<8;go++) { SinglePixel(100); }
}


void RandomSparkle(){
  int COLOR = Color(TrueRandom.random(0,255), TrueRandom.random(0,255), TrueRandom.random(0,255));
  int DENSITY = TrueRandom.random(1,30);
  int DELAY = TrueRandom.random(25,160);
  
  sparkle(COLOR, DENSITY, DELAY);  
}

void ColorJump(uint8_t DELAY){
  int i;
  uint32_t COLOR;
  COLOR = RandomColor();
  for(i=0;i<strip.numPixels();i++) { q(i, COLOR); }
  strip.show();
  delay(DELAY/2);
  for(i=0;i<strip.numPixels();i++) { q(i, Color(0,0,0)); }
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
		// strip.setPixelColor(i, Color(0,0,0));
    q(i, Color(0,0,0));
  }
  strip.show();
  
  for( r=0; r < DENSITY; r++ )  {
   PIXEL = TrueRandom.random(1,Total);
   // strip.setPixelColor(PIXEL, COLOR);
	 q(PIXEL, COLOR);
  }
  strip.show();
  
  delay(DELAY);
 //
 
}

uint32_t RandomColor(){
   return Color(TrueRandom.random(0,255), TrueRandom.random(0,255), TrueRandom.random(0,255));
}

// "Queue" method to translate pixel positions for standard, mirrored, and radial modes.
void q(int pos, uint32_t color) {
		
	if (division == 1) {
		strip.setPixelColor(pos,color);
	} else if (division == 2) {
		// left/right mirror mode.
		mirror(pos,color);
	} else if (division == 4	) {
		// Radial Mode
		// radial(pos,color);
	}
	
}

// Translate the LED position to Mirror the strand output.
void mirror(int pos, uint32_t color) {

// Left vs Right alignment for forward section
// Left 0 = 11          // Right 0 = 12
// Left 1 = 10          // Right 1 = 13
// Left 2 = 9           // Right 2 = 14
// Left 3 = 8           // Right 3 = 15
// Left 4 = 7           // Right 4 = 16
// Left 5 = 6           // Right 5 = 17
// Left 6 = 5           // Right 6 = 18
// Left 7 = 4           // Right 7 = 29
// Left 8 = 3           // Right 8 = 20
// Left 9 = 2           // Right 9 = 21 
// Left 10 = 1          // Right 10 = 0	
												
// Left 11 = 27         // Right 11 = 30
// Left 12 = 26         // Right 12 = 31
// Left 13 = 25         // Right 13 = 32
// Left 14 = 24         // Right 14 = 33
// Left 15 = 23         // Right 15 = 34
// Left 16 = 22         // Right 16 = 35

                                       
// Left 17 = 72           // Right 17 = 36
// Left 18 = 71           // Right 18 = 37
// Left 19 = 70           // Right 19 = 38
// Left 20 = 69           // Right 20 = 39
// Left 21 = 68         // Right 21 = 40
// Left 22 = 67         // Right 22 = 41


// Left 23 = 66         // Right 23 = 42
// Left 24 = 65         // Right 24 = 43
// Left 25 = 64         // Right 25 = 44

// Left 6 = 5           // R 27 = 62
// Left 6 = 5           // R 28 = 63
// Left 7 = 4           // R 29 = 62
												// R 30 = 61			
// Left 6 = 5           // R 31 = 60
// Left 7 = 4           // R 32 = 59
// Left 8 = 3           // R 33 = 58
// Left 9 = 2           // R 34 = 57 
// Left 10 = 1          // R 35 = 56





// Center = 12 /// For a radial translation
// Center = 30 /// For a radial translation

	int left,right;
	
	if (pos >= strip.numPixels()/2) {
		// pos = (pos / 2) + 
		// Serial.println(pos);
	}
 	
	if (pos >= 0 && pos <= 10) {
		left = 11 - pos;
		right = 12 + pos;
		if (pos == 10) { right = 0; }
	}	

	if (pos >= 11 && pos <= 17) {
		left = 40 - pos;
		right = 19 + pos;
	}	
	
  if (pos >= 18 && pos <= 23) {
		left = 90 - pos;
		right = 19 + pos;
	}
	
	if (pos >= 24 && pos <= 26) {
	// 	left = 41 - pos;
	// 	right = 21 + pos;				
		// if (pos == 25) { left = 49; }
		return;
	}
	
	if (pos >= 27 && pos <= 35) {
		// R = 56 - 9  47 = 27
		left = 20 + pos;
		right = 91 - pos;
				
		// if (pos == 25) { left = 49; }
	}	
	
  // strip.setPixelColor(left, Color(255,0,255));
  // strip.setPixelColor(right, Color(0,255,255));
  strip.setPixelColor(left, color);
  strip.setPixelColor(right, color);
}


void rainbow(uint8_t wait) {
  int i, j, k;
   
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255,1));
          // Serial.println((i + j) % 255);
          // Serial.println(Wheel( (i + j) % 255,1));
    }  
    strip.show();   // write all the pixels out 
    delay(wait);
  }
  
  
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
	int i, j;
	double a=1;
  
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
	
    for (i=0; i < strip.numPixels()/division; i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      q(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256,a) );
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
      q(i, c);
      strip.show();
      delay(wait);
  }
}

/* Helper functions */

uint32_t rgba(byte r, byte g, byte b, double a) {
	
	int rr = r*a;
	int gg = g*a;
	int bb = b*a;
	
	return Color(rr,gg,bb);
}

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
uint32_t Wheel(byte WheelPos, double a)
{
  if (WheelPos < 85) {
   return rgba(WheelPos * 3, 255 - WheelPos * 3, 0,a);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return rgba(255 - WheelPos * 3, 0, WheelPos * 3,a);
  } else {
   WheelPos -= 170; 
   return rgba(0, WheelPos * 3, 255 - WheelPos * 3,a);
  }
}
