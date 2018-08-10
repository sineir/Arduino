/*

stm32f103 - level adapter 3.3v 5v - max7219



*/

#include "LedControl.h"
#include "Wire.h"

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}
/* 
 * Create a new controler 
 * Params :
 * int dataPin    The pin on the Arduino where data gets shifted out
 * int clockPin   The pin for the clock
 * int csPin      The pin for selecting the device when data is to be sent
 * int numDevices The maximum number of devices that can be controled
*/
// Create a LedControl for 4 devices... 
//LedControl(int dataPin, int clkPin, int csPin, int numDevices);
#define     N_DEV   1
// pinos fisicos atmega8  18.17.16
//LedControl lc1=LedControl(12,11,10,N_DEV);
/*
     DIN       -> MOSI    (Arduino output)
     CLK       -> SCK     (Arduino output)
     LOAD/#CS  -> SS      (Arduino output)
*/
LedControl lc1=LedControl(PA7,PA5,PA4,N_DEV); // para STM32F103C8
//LedControl(int dataPin, int clkPin, int csPin, int numDevices);
int uni,dez,cen,mil;
int ctr=1;
boolean dp1,dp2,dp3,dp4 = false;
int teste=0;

void setup() {
    Wire.begin();
  // put your setup code here, to run once:
   //wake up the MAX72XX from power-saving mode 
   lc1.shutdown(0,false);
   //set a medium brightness for the Leds
  Serial1.begin(9600);
  //we have already set the number of devices when we created the LedControl
  int devices=lc1.getDeviceCount();
  //we have to init all devices in a loop
  for(int address=0;address<devices;address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc1.shutdown(address,false);
    /* Set the brightness to a medium values */
    lc1.setIntensity(address,8);
    /* and clear the display */
    lc1.clearDisplay(address);
  }
   //mil = 9; cen=8;
   uni=dez=cen=mil=0;
   dp1=true;
   //second,min,hour,day,date,month,year
   //setDS3231time(0,47,12,1,16,10,16);
}

void loop() {

//lc1.setIntensity(0,ctr);

  dp1=ctr==1;
  dp2=ctr==2;
  dp3=ctr==3;
  dp4=ctr==4;


//dp1=! dp1;

        if (uni==10){
          uni=0;
          dez++;
        }
        if (dez==10){
          dez=0;
          cen++;
          teste=1;
          }
        if (cen==10){
          cen=0;
          mil++;
          }
        if (mil==10){
          mil=0;
          }

      lc1.setDigit(0, 3, uni, dp1);
      lc1.setDigit(0, 2, dez, dp2);
      lc1.setDigit(0, 1, cen, dp3);
      lc1.setDigit(0, 0, mil, dp4);
      lc1.setDigit(0, 7, uni, dp1);
      lc1.setDigit(0, 6, dez, dp2);
      lc1.setDigit(0, 5, cen, dp3);
      lc1.setDigit(0, 4, mil, dp4);
    Serial1.println(uni);
  if (teste == 1 ){
    for (int j=0;j<10;j++) {
    lc1.setDigit(0, 3, 15, 0);delay(100);
    for (int i=0;i<8;i++){
      lc1.setLed(0,3,i,false);
    }
        delay(100);
    }
    teste=0;
    delay(1000);
    lc1.clearDisplay(0);
    for (int k=0;k<8;k++){
    for (int j=0;j<5;j++){
    for (int i=0;i<8;i++){
     lc1.setLed(0,k,i,true);
     delay(50);
     lc1.setLed(0,k,i,false);
    }
    }
    }

    
    for (int k=0;k<8;k++){
     lc1.setLed(0,k,0,true);
     lc1.setDigit(0, 7, k, dp4);
     delay(1000);
     lc1.setLed(0,k,0,false);
    
    
    }

    delay(1000);
    lc1.clearDisplay(0);
    // L
    lc1.setLed(0,0,4,true);
    lc1.setLed(0,0,5,true);
    lc1.setLed(0,0,6,true);
    // U
    lc1.setLed(0,1,2,true);
    lc1.setLed(0,1,3,true);
    lc1.setLed(0,1,4,true);
    lc1.setLed(0,1,5,true);
    lc1.setLed(0,1,6,true);
    // C
    lc1.setLed(0,2,1,true);
    lc1.setLed(0,2,4,true);
    lc1.setLed(0,2,5,true);
    lc1.setLed(0,2,6,true);
    // A
    lc1.setLed(0,3,2,true);
    lc1.setLed(0,3,3,true);
    lc1.setLed(0,3,1,true);
    lc1.setLed(0,3,5,true);
    lc1.setLed(0,3,6,true);
    lc1.setLed(0,3,7,true);
    // S
    lc1.setLed(0,4,1,true);
    lc1.setLed(0,4,3,true);
    lc1.setLed(0,4,4,true);
    lc1.setLed(0,4,6,true);
    lc1.setLed(0,4,7,true);
    delay(3000);
    lc1.clearDisplay(0);
    // S
    lc1.setLed(0,0,1,true);
    lc1.setLed(0,0,3,true);
    lc1.setLed(0,0,4,true);
    lc1.setLed(0,0,6,true);
    lc1.setLed(0,0,7,true);
    // A
    lc1.setLed(0,1,2,true);
    lc1.setLed(0,1,3,true);
    lc1.setLed(0,1,1,true);
    lc1.setLed(0,1,5,true);
    lc1.setLed(0,1,6,true);
    lc1.setLed(0,1,7,true);
    // r
    lc1.setLed(0,2,5,true);
    lc1.setLed(0,2,7,true);
    // A
    lc1.setLed(0,3,2,true);
    lc1.setLed(0,3,3,true);
    lc1.setLed(0,3,1,true);
    lc1.setLed(0,3,5,true);
    lc1.setLed(0,3,6,true);
    lc1.setLed(0,3,7,true);
  
    delay(3000);
    }


        delay(100);
  uni++;
  ctr++;
    if (ctr==5) ctr=1;

}
