// All the mcufriend.com UNO shields have the same pinout.
// i.e. control pins A0-A4.  Data D2-D9.  microSD D10-D13.
// Touchscreens are normally A1, A2, D7, D6 but the order varies
//
// This demo should work with most Adafruit TFT libraries
// If you are not using a shield,  use a full Adafruit constructor()
// e.g. Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
/*
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
*/

#define LCD_CS PB4 // Chip Select goes to Analog 3
#define LCD_CD PB5 // Command/Data goes to Analog 2
#define LCD_WR PB6 // LCD Write goes to Analog 1
#define LCD_RD PB7 // LCD Read goes to Analog 0
#define LCD_RESET PB3 // Can alternately just connect to Arduino's reset pin


#include <SPI.h>       
// f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <Fonts/FreeMono9pt7b.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
//#include <Adafruit_TFTLCD.h>
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup(void) {
    Serial.begin(9600);
    uint32_t when = millis();
    //    while (!Serial) ;   //hangs a Leonardo until you connect a Serial
    if (!Serial) delay(5000);           //allow some time for Leonardo
    Serial.println("Serial took " + String((millis() - when)) + "ms to start");
    //    tft.reset();                 //hardware reset
    uint16_t ID = tft.readID(); //
    Serial.print("ID = 0x");
    Serial.println(ID, HEX);
    if (ID == 0xD3D3) ID = 0x9481; // write-only shield
//    ID = 0x9329;                             // force ID
    tft.begin(ID);
}

uint16_t scrollbuf[320];    // my biggest screen is 320x480
#define READGRAM(x, y, buf, w, h)  tft.readGRAM(x, y, buf, w, h)

void loop(void){

    uint8_t aspect;
    uint16_t pixel;
    const char *aspectname[] = {
        "PORTRAIT", "LANDSCAPE", "PORTRAIT_REV", "LANDSCAPE_REV"
    };
    const char *colorname[] = { "BLUE", "GREEN", "RED", "GRAY" };
    uint16_t colormask[] = { 0x001F, 0x07E0, 0xF800, 0xFFFF };
    uint16_t dx, rgb, n, wid, ht, msglin;
    tft.setRotation(0);

    unsigned long start;
    delay(1000);
    tft.fillScreen(BLACK);
    //start = micros();
    /*
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);  tft.setTextSize(1);
    tft.println("Hello World!");
    tft.setTextColor(YELLOW); tft.setTextSize(2);
    tft.println(123.45);
    tft.setTextColor(RED);    tft.setTextSize(3);
    tft.println(0xDEADBEEF, HEX);
    tft.println();
    tft.setTextColor(GREEN);
    tft.setTextSize(5);
    tft.println("Big 5");
    delay(5000);
    */
  tft.setRotation(1);
  int lin = 10;
  int col = 40;
  int larg = 300;
  int altu = 50;
  int radius = 10;
  
  tft.fillRoundRect(col, lin, larg, altu, radius, YELLOW);
  tft.fillRoundRect(col+5, lin+5, larg-10, altu-10, radius, BLUE);
  tft.setCursor(col+20, lin+13);
  tft.setTextColor(YELLOW); tft.setTextSize(3);
  tft.println("Volume");

  lin += 50;
  tft.fillRoundRect(col, lin, larg, altu, radius, YELLOW);
  tft.fillRoundRect(col+5, lin+5, larg-10, altu-10, radius, BLUE);
  tft.setCursor(col+20, lin+13);
  tft.setTextColor(YELLOW); tft.setTextSize(3);
  tft.println("Graves");
  tft.setTextColor(RED);
  
  lin += 50;
  tft.setTextSize(3);
  for (int i=0;i<280;i++) {
  tft.setCursor(col+i, lin);
    tft.print(">");
    delay(10);
    }

  
  //tft.fillRoundRect(20, 100, 300, 50, 10, YELLOW);
  //tft.fillRoundRect(25, 105, 290, 40, 10, BLUE);
  /*
  tft.setCursor(40, 113);
  tft.setTextColor(YELLOW); tft.setTextSize(3);
  tft.println("Lucas Soares");
  */
    delay(5000);

  }

