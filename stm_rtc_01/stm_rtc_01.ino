/*
  placa stm32f103 pill blue generic: https://github.com/stm32duino/Arduino_Core_STM32
  biblioteca: https://github.com/stm32duino/STM32RTC
  display: max7219
*/


/**
  ******************************************************************************
    @file    SimpleRTC.ino
    @author  WI6LABS
    @version V1.0.0
    @date    12-December-2017
    @brief   Simple RTC example.

  ******************************************************************************
    @attention

    <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:
      1. Redistributions of source code must retain the above copyright notice,
         this list of conditions and the following disclaimer.
      2. Redistributions in binary form must reproduce the above copyright notice,
         this list of conditions and the following disclaimer in the documentation
         and/or other materials provided with the distribution.
      3. Neither the name of STMicroelectronics nor the names of its contributors
         may be used to endorse or promote products derived from this software
         without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  ******************************************************************************
*/
#include "LedControl.h"
#include "Wire.h"
#define     N_DEV   1
LedControl lc1 = LedControl(PA7, PA5, PA4, N_DEV); // para STM32F103C8

#include <STM32RTC.h>

/* Get the rtc object */
STM32RTC& rtc = STM32RTC::getInstance();

/* Change these values to set the current initial time */
// 23:20:00
const byte seconds = 0;
const byte minutes = 20;
const byte hours = 23;

/* Change these values to set the current initial date */
/* Monday 15th June 2015 */
//11/08/2018 SAT
const byte weekDay = 7;
const byte day = 11;
const byte month = 8;
const byte year = 18;
boolean dp1, dp2, dp3, dp4 = false; //digital point 'on' 'off'
int hora, minuto, segundo = 0;
void setup()
{
  Serial.begin(9600);
  rtc.setClockSource(STM32RTC::RTC_HSE_CLOCK);
  rtc.begin(); // initialize RTC 24H format
  pinMode(PB12, INPUT_PULLUP);
  Wire.begin();
  // put your setup code here, to run once:
  //wake up the MAX72XX from power-saving mode
  lc1.shutdown(0, false);
  int devices = lc1.getDeviceCount();
  //we have to init all devices in a loop
  for (int address = 0; address < devices; address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc1.shutdown(address, false);
    /* Set the brightness to a medium values */
    lc1.setIntensity(address, 8);
    /* and clear the display */
    lc1.clearDisplay(address);
  }


  // Set the date
  rtc.setWeekDay(weekDay);
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);
  /*
    lc1.setDigit(0, 1, year, dp3);
    lc1.setDigit(0, 0, mil, dp4);
    lc1.setDigit(0, 7, uni, dp1);
    lc1.setDigit(0, 6, dez, dp2);
    lc1.setDigit(0, 5, cen, dp3);
    lc1.setDigit(0, 4, mil, dp4);
  */
  // you can use also
  //rtc.setTime(hours, minutes, seconds);
  //rtc.setDate(weekDay, day, month, year);
}

void loop()
{

  if (digitalRead(PB12) == 0) {
    // Set the time 14:07:00
    rtc.setHours(14);
    rtc.setMinutes(7);
    rtc.setSeconds(0);
  }
  // Print date...
  print2digits(rtc.getDay());
  Serial.print("/");
  print2digits(rtc.getMonth());
  Serial.print("/");
  print2digits(rtc.getYear());
  Serial.print(" ");

  // ...and time
  print2digits(rtc.getHours());
  Serial.print(":");
  print2digits(rtc.getMinutes());
  Serial.print(":");
  print2digits(rtc.getSeconds());

  hora = rtc.getHours();
  minuto = rtc.getMinutes();
  segundo = rtc.getSeconds();


  Serial.println();

  lc1.setDigit(0, 7, hora / 10, dp3);
  lc1.setDigit(0, 6, hora % 10, dp4);
  lc1.setLed(0, 5, 7, true);
  lc1.setDigit(0, 4, minuto / 10, dp3);
  lc1.setDigit(0, 3, minuto % 10, dp4);
  lc1.setLed(0, 2, 7, true);
  lc1.setDigit(0, 1, segundo / 10, dp3);
  lc1.setDigit(0, 0, segundo % 10, dp4);

  delay(1000);
}



void print2digits(int number) {
  if (number < 10) {
    Serial.print("0"); // print a 0 before if the number is < than 10
  }
  Serial.print(number);
}