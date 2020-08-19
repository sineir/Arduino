# Arduino
Meus sketches Arduino e correlatos stm
 1. [Relogio 4 digitos ajuste via botoes e modulo bluetooth(appinventor soft)](https://github.com/sineir/Arduino/tree/master/Relogio_Max7219_4dig_bt)
2. [Relogio 4 digitos ajuste via controle remoto](https://github.com/sineir/Arduino/tree/master/Relogio_Max7219_4dig_ir)
3. [ESP8266 iot mqttcloud um botao e um rele(moc+bta)](https://github.com/sineir/Arduino/tree/master/easyiot_button_relay)
4. [Contador digital, teste stm32 e max7219](https://github.com/sineir/Arduino/tree/master/stm32_Max7219)
5. [Stm32 com display ILI9341: projeto menu com rotary(em andamanto)](https://github.com/sineir/Arduino/tree/master/stm32_ili9341)

[Contador max7219](https://www.youtube.com/watch?v=GsAP-7-6Gko)
```
#include "LedControl.h"
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
#define     N_DEV   4 
LedControl lc1=LedControl(11,13,10,N_DEV);
int uni,dez,cen,mil;
// digitos 0 - 9
byte digitos[10][8]= {
{ B0011100,
  B0100010,
  B0100010,
  B0100010,
  B0100010,
  B0100010,
  B0100010,
  B0011100
  },
{ B0001000,
  B0011000,
  B0001000,
  B0001000,
  B0001000,
  B0001000,
  B0001000,
  B0011100
  },
{ B0011100,
  B0100010,
  B0000010,
  B0000010,
  B0011100,
  B0100000,
  B0100000,
  B0111110
  },
{ B0011100,
  B0000010,
  B0000010,
  B0011100,
  B0000010,
  B0000010,
  B0000010,
  B0011100
  },
{ B0100010,
  B0100010,
  B0100010,
  B0011110,
  B0000010,
  B0000010,
  B0000010,
  B0000010
  },
{ B0111100,
  B0100000,
  B0100000,
  B0111000,
  B0000100,
  B0000100,
  B0000100,
  B0111000
  },
{ B0011100,
  B0100010,
  B0100000,
  B0100000,
  B0111100,
  B0100010,
  B0100010,
  B0011100
  },
{ B0011110,
  B0000010,
  B0000010,
  B0000100,
  B0001000,
  B0001000,
  B0001000,
  B0001000
  },
{ B0011100,
  B0100010,
  B0100010,
  B0100010,
  B0011100,
  B0100010,
  B0100010,
  B0011100
  },
{ 
  B0011100,
  B0100010,
  B0100010,
  B0011110,
  B0000010,
  B0000010,            
  B0100010,
  B0011100
}
  };

void setup() {
  // put your setup code here, to run once:
   //wake up the MAX72XX from power-saving mode 
   lc1.shutdown(0,false);
   //set a medium brightness for the Leds
   lc1.setIntensity(0,8);

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
   mil = 9; cen=8;
}

void loop() {
  // put your main code here, to run repeatedly:
  //........dev,row,col, true=ON false=OFF
  //lc.setLed(0,2,7,true);
  //uni,dez,cen,mil;
    for (int m=0;m<8;m++) {
        lc1.setRow(3,m,digitos[uni][m]); // dev, row , line
        lc1.setRow(2,m,digitos[dez][m]); // dev, row , line
        lc1.setRow(1,m,digitos[cen][m]); // dev, row , line
        lc1.setRow(0,m,digitos[mil][m]); // dev, row , line
    }
delay(10);
uni++;
if (uni==10){
  uni=0;
  dez++;
}
if (dez==10){
  dez=0;
  cen++;
}
if (cen==10){
  cen=0;
  mil++;
}
if (mil==9 && cen==9 && dez==9 && uni==9) pisca();
if (mil==10){
  mil=0;
  cen=0;
  dez=0;
  uni=0;
}

}

void blank(){
        for (int k=0;k<N_DEV;k++)
        lc1.clearDisplay(k);
  }

void pisca(){
for(int ll=0;ll<10;ll++){
      for (int m=0;m<8;m++){
        lc1.setRow(0,m,digitos[mil][m]); // dev, row , line
        lc1.setRow(1,m,digitos[cen][m]); // dev, row , line
        lc1.setRow(2,m,digitos[dez][m]); // dev, row , line
        lc1.setRow(3,m,digitos[uni][m]); // dev, row , line
      }
      delay(100);
      blank();
      delay(100);
}
}
```
