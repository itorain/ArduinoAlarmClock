/*this is a modified demo code where i was playing around with the display to get it to display different things. 
I added my own font for letters.
#### Pins  
+ A, B, C, D - line select for two 74HC138
+ OE - Ouput Enable(active low) for 74HC595
+ STB - data latch for 74HC595
+ CLK - shift register clock input for 74HC595
*/
#include <LEDMatrix.h>
#include "LEDMatrix.h"
#include <TimerOne.h>

#define WIDTH   32
#define HEIGHT  16

//const uint8_t* a = (uint8_t*)&value;

const uint8_t digits[] = {
0x00,0x1C,0x36,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x36,0x1C,0x00,0x00,0x00,0x00, // 0
0x00,0x18,0x78,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,0x00,0x00,0x00, // 1
0x00,0x3E,0x63,0x63,0x63,0x06,0x06,0x0C,0x18,0x30,0x63,0x7F,0x00,0x00,0x00,0x00, // 2
0x00,0x3E,0x63,0x63,0x06,0x1C,0x06,0x03,0x03,0x63,0x66,0x3C,0x00,0x00,0x00,0x00, // 3
0x00,0x06,0x0E,0x1E,0x36,0x36,0x66,0x66,0x7F,0x06,0x06,0x1F,0x00,0x00,0x00,0x00, // 4
0x00,0x7F,0x60,0x60,0x60,0x7C,0x76,0x03,0x03,0x63,0x66,0x3C,0x00,0x00,0x00,0x00, // 5
0x00,0x1E,0x36,0x60,0x60,0x7C,0x76,0x63,0x63,0x63,0x36,0x1C,0x00,0x00,0x00,0x00, // 6
0x00,0x7F,0x66,0x66,0x0C,0x0C,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00, // 7
0x00,0x3E,0x63,0x63,0x63,0x36,0x1C,0x36,0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00, // 8
0x00,0x1C,0x36,0x63,0x63,0x63,0x37,0x1F,0x03,0x03,0x36,0x3C,0x00,0x00,0x00,0x00, // 9
};
const uint8_t letters[] = { 0x00,0x7c,0x44,0x44,0x7c,0x44,0x00,0x00, //a
  0x00,0x7c,0x44,0x78,0x44,0x7c,0x00,0x00, //b  
  0x00,0x7c,0x40,0x40,0x40,0x7c,0x00,0x00, //c  
  0x00,0x78,0x44,0x44,0x44,0x78,0x00,0x00, //d 
  0x00,0x7c,0x40,0x78,0x40,0x7c,0x00,0x00, //e
  0x00,0x7c,0x40,0x70,0x40,0x40,0x00,0x00, //f
  0x00,0x7c,0x40,0x4c,0x44,0x7c,0x00,0x00, //g
  0x00,0x44,0x44,0x7c,0x44,0x44,0x00,0x00, //h
  0x00,0x7c,0x10,0x10,0x10,0x7c,0x00,0x00, //i
  0x00,0x0c,0x04,0x04,0x44,0x7c,0x00,0x00, //j
  0x00,0x44,0x48,0x70,0x48,0x44,0x00,0x00, //k
  0x00,0x40,0x40,0x40,0x40,0x7c,0x00,0x00, //l
  0x00,0x44,0x6c,0x54,0x44,0x44,0x00,0x00, //m
  0x00,0x44,0x64,0x54,0x4c,0x44,0x00,0x00, //n
  0x00,0x38,0x44,0x44,0x44,0x38,0x00,0x00, //o
  0x00,0x78,0x44,0x78,0x40,0x40,0x00,0x00, //p              
  0x00,0x7c,0x44,0x44,0x7c,0x10,0x00,0x00, //q            
  0x00,0x78,0x44,0x78,0x44,0x44,0x00,0x00, //r            
  0x00,0x7c,0x40,0x7c,0x04,0x7c,0x00,0x00, //s            
  0x00,0x7c,0x10,0x10,0x10,0x10,0x00,0x00, //t              
  0x00,0x44,0x44,0x44,0x44,0x7c,0x00,0x00, //u            
  0x00,0x44,0x44,0x28,0x28,0x10,0x00,0x00, //v            
  0x00,0x44,0x44,0x54,0x54,0x28,0x00,0x00, //w            
  0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x00, //x            
  0x00,0x44,0x44,0x28,0x10,0x10,0x00,0x00, //y            
  0x00,0x7c,0x08,0x10,0x20,0x7c,0x00,0x00, //z
};

//const uint8_t a[] = {0x00,0x7c,0x44,0x44,0x7c,0x44,0x00,0x00};

LEDMatrix matrix(2, 3, 4, 5, 6, 11, 10, 13);     // LEDMatrix(a, b, c, d, oe, r1, stb, clk);
uint8_t displaybuf[WIDTH *HEIGHT / 8];          // Display Buffer what does this do?

void timer_isr() {
    matrix.scan();
}
void setup() {
    Serial.begin(115200);
    matrix.begin(displaybuf, WIDTH, HEIGHT);
    Timer1.initialize(1200);
    Timer1.attachInterrupt(timer_isr);

    delay(1500);
    matrix.reverse();
    delay(1500);
    matrix.reverse();
    matrix.clear();
}

void loop() {
    static uint32_t lastCountTime = 0;
    static uint8_t count = 0;
    matrix.drawDigits(0,0,1); //set tens place of the hour
    matrix.drawDigits(8,0,1); //set ones place of the hour
    matrix.drawDigits(16,0,4); //set tens place of the minutes
    matrix.drawDigits(24,0,1); //set ones place of the minutes
    matrix.drawPoint(15,7,1); //draw dots for clock
    matrix.drawPoint(15,4,1); 
    //matrix.scan();
   //matrix.drawImage(8,0,5,5,a);
    //if ((millis() - lastCountTime) > 3000) {
       // lastCountTime = millis();
        //matrix.drawRect(count + 4, count, 28 - count, 16 - count, 1 - (count & 1));
       // count = (count + 1) & 0x0F;
    //} displays a pattern of rectangles 
   //for(count; count <13;count++){
     //drawLetters(24, 0,count); displays alphabet two letters at a time
     //drawDigits(8, 0, count); counts through the digits starting with 0
   //delay(1000);
   //}
   //count =0;
}



