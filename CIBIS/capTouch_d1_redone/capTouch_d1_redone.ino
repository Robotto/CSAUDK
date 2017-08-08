#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#include "Wire.h"
#include "Adafruit_FT6206.h"

Adafruit_FT6206 ctp = Adafruit_FT6206();

// DC/CS
#define TFT_DC D4
#define TFT_CS D8

// Cap Touch stuff!

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Size of the color selection boxes and the paintbrush size  
#define BOXSIZE 40
int PENRADIUS = 5;
int oldcolor, currentcolor;

void setup() {

  Serial.begin(115200);
  Serial.println("Up!");

  Serial.print("height (y): ");
  Serial.println(tft.height());
 Serial.print("width (x): ");
  Serial.println(tft.width());

  // put your setup code here, to run once:
  tft.begin();

    if (! ctp.begin(40)) {  // pass in 'sensitivity' coefficient
    Serial.println("Couldn't start FT6206 touchscreen controller");
    while (1);
  }

  Serial.println("Capacitive touchscreen started");

  tft.fillScreen(ILI9341_BLACK);


  
  // make the color selection boxes
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
  tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);
  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE);
  tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);

  // brushsize boxes
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
  tft.fillRect(0, tft.height()-BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_DARKGREY);
  tft.setCursor(BOXSIZE/2-4, tft.height()-BOXSIZE/2-6);
  tft.println("-");
  
  tft.fillRect(BOXSIZE, tft.height()-BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_LIGHTGREY);
  tft.setCursor(BOXSIZE+BOXSIZE/2-4, tft.height()-BOXSIZE/2-6);
  tft.println("+");

  // select the current color 'red'
  currentcolor = ILI9341_RED;


  showCursorStatus();
  

  // clear screen box
  tft.fillRect(BOXSIZE*5, tft.height()-BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);  
  tft.setCursor(BOXSIZE*5+BOXSIZE/2-10, tft.height()-BOXSIZE/2-6);
  tft.println("CL");
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);

  // transmit button
  tft.fillRect(BOXSIZE*4, tft.height()-BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(BOXSIZE*4+BOXSIZE/2-10, tft.height()-BOXSIZE/2-6);
  tft.println("TX");
  tft.drawRect(0,0, BOXSIZE, BOXSIZE, ILI9341_WHITE);


  
}

void loop() {
  // Wait for a touch
  if (! ctp.touched()) {
    return;
  }

  // Retrieve a point  
  TS_Point p = ctp.getPoint();
  
 /*
  // Print out raw data from screen touch controller
  Serial.print("X = "); Serial.print(p.x);
  Serial.print("\tY = "); Serial.print(p.y);
  Serial.print(" -> ");
 */

  // flip it around to match the screen.
  p.x = map(p.x, 0, 240, 240, 0);
  p.y = map(p.y, 0, 320, 320, 0);

  // Print out the remapped (rotated) coordinates
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");

  if(p.y > tft.height()-BOXSIZE && p.y < tft.height()){
    // vi er i selection menuen
    if(p.x < BOXSIZE){
      if(PENRADIUS > 1){
       PENRADIUS = PENRADIUS-1; 
       Serial.print("Pen Radius -- = ");
       Serial.println(PENRADIUS);
       showCursorStatus();
       delay(500);
      }
    } else if(p.x < BOXSIZE*2){
       PENRADIUS = PENRADIUS+1; 
       Serial.print("Pen Radius ++ = ");
       Serial.println(PENRADIUS);
       showCursorStatus();
       delay(500);
    }
    else if(p.x > tft.width()-BOXSIZE) //clear screen
    {
       Serial.println("Clear screen");
       tft.fillRect(0,BOXSIZE, tft.height()-2*BOXSIZE, tft.width(), ILI9341_BLACK);
       //tft.fillScreen(ILI9341_BLACK);
    }

  }

  if (p.y < BOXSIZE) {
     oldcolor = currentcolor;

     if (p.x < BOXSIZE) { 
       currentcolor = ILI9341_RED; 
       tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
     } else if (p.x < BOXSIZE*2) {
       currentcolor = ILI9341_YELLOW;
       tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
     } else if (p.x < BOXSIZE*3) {
       currentcolor = ILI9341_GREEN;
       tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
     } else if (p.x < BOXSIZE*4) {
       currentcolor = ILI9341_CYAN;
       tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
     } else if (p.x < BOXSIZE*5) {
       currentcolor = ILI9341_BLUE;
       tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
     } else if (p.x <= BOXSIZE*6) {
       currentcolor = ILI9341_MAGENTA;
       tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
     }

     if (oldcolor != currentcolor) {
        if (oldcolor == ILI9341_RED) 
          tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
        if (oldcolor == ILI9341_YELLOW) 
          tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
        if (oldcolor == ILI9341_GREEN) 
          tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
        if (oldcolor == ILI9341_CYAN) 
          tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);
        if (oldcolor == ILI9341_BLUE) 
          tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE);
        if (oldcolor == ILI9341_MAGENTA) 
          tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);

        showCursorStatus();
     }
  }
  if (((p.y-PENRADIUS) > BOXSIZE) && (p.y+PENRADIUS) < tft.height()-BOXSIZE && ((p.y+PENRADIUS) < tft.height())) {
    tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
  }

}


void showCursorStatus()
{  //show cursor status:
  tft.fillRect(BOXSIZE*2, tft.height()-BOXSIZE, BOXSIZE*2, BOXSIZE, ILI9341_BLACK);   //clear the area first
  tft.setCursor(2*BOXSIZE+BOXSIZE/2, tft.height()-BOXSIZE);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Pen: ");
  tft.setCursor(2*BOXSIZE+BOXSIZE/2+4, tft.height()-BOXSIZE+18);
  tft.setTextColor(currentcolor);  tft.setTextSize(2);
  tft.print(PENRADIUS);
  tft.print("px");
}
