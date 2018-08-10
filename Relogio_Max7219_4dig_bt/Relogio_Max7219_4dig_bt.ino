#include "LedControl.h"
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
//#include <SoftwareSerial.h>
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
LedControl lc1=LedControl(12,11,10,N_DEV);
//SoftwareSerial Serial2(8, 9); //rx,tx
int resp=0;
int ctr=0;
boolean ajuste=false;
String inData;
String inComand;
void setup() {
    Wire.begin();
    Serial.begin(9600);
  //  Serial2.begin(9600);
  // put your setup code here, to run once:
   //wake up the MAX72XX from power-saving mode 
   lc1.shutdown(0,false);
   //set a medium brightness for the Leds
   lc1.setIntensity(0,12);

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
  // mil = 9; cen=8;
   //second,min,hour,day,date,month,year
   //setDS3231time(0,55,22,5,29,6,17);
}

void loop() {
 byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,&year);
  //month=5; dayOfMonth=30;  year=17;  second=30;  ctr=0;
//lc1.setIntensity(0,second%15);
  if (second == 30 && ctr == 0 ){

      lc1.setDigit(0, 0, month%10, false);
      lc1.setDigit(0, 1, month/10, false);
      lc1.setDigit(0, 2, dayOfMonth%10, false);
      lc1.setDigit(0, 3, dayOfMonth/10, false);
      ctr = 1 ;
      //lc1.setDigit(0, 4, year%10, false);
      //lc1.setDigit(0, 5, year/10, false);
      //lc1.setDigit(0, 2, 0, false);
      //lc1.setDigit(0, 3, 2, false); 
            delay(2500);
             lc1.clearDisplay(0);
     } 
  
  if (second == 40  ) ctr = 0;
  
      lc1.setDigit(0, 0, minute%10, false);
      lc1.setDigit(0, 1, minute/10, false);
      lc1.setDigit(0, 2, hour%10, false);
        if (hour>9){
       lc1.setDigit(0, 3, hour/10, false);
        }else{
       lc1.setDigit(0, 3, 16, false);
        }
      //lc1.setDigit(0, 4, second%10, false);
      //lc1.setDigit(0, 5, second/10, false);
        delay(250);
    if (hour>18 and hour < 6){
      lc1.setIntensity(0,8);
    }else{
      lc1.setIntensity(0,14);
    }
  // put your main code here, to run repeatedly:
  //........dev,row,col, true=ON false=OFF
  //lc.setLed(0,2,7,true);
  //uni,dez,cen,mil;
    int vhr,vmin,vsec,vday,vdate,vmes,vano;
    resp=0;
     while (Serial.available() > 0)
    {
        char recieved = Serial.read();
        if (isDigit(recieved)) {
                inData += recieved;
        }else{
          inComand+= recieved;
          }

      if (inComand.substring(0,7) == "gettime" and resp == 0 ) {
        //byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
      Serial.print(hour/10);
      Serial.print(hour%10);
      Serial.print(":"); 
      Serial.print(minute/10); 
      Serial.print(minute%10);
      Serial.print(":");
      Serial.print(second/10); 
      Serial.print(second%10);
      Serial.print(" ");
      Serial.print(dayOfMonth/10);
      Serial.print(dayOfMonth%10);
      Serial.print("/");
      Serial.print(month/10);
      Serial.print(month%10);
      Serial.print("/20");
      Serial.print(year/10);
      Serial.print(year%10);
      Serial.print(" "); 
      Serial.print(dayOfWeek);
        resp=1;
        }

      
      //if (recieved == '\n') {
      //hhmmssDddmmyy
      //setDS3231time(0,55,22,5,29,6,17);
      if (inData.length() == 13) {
      //2349002906175 hr min sec dia mes ano dia.semana
      vhr  = inData.substring(0,2).toInt();
      vmin = inData.substring(2,4).toInt();
      vsec = inData.substring(4,6).toInt();
      vdate= inData.substring(6,8).toInt();
      vmes = inData.substring(8,10).toInt();
      vano = inData.substring(10,12).toInt();
      vday = inData.substring(12).toInt();
      Serial.print(vhr);    Serial.print(":"); // hora
      Serial.print(vmin);   Serial.print(":"); //minuto
      Serial.print(vsec);   Serial.print(" "); //segundo
      Serial.print(vday);   Serial.print(" "); //dia da semana
      Serial.print(vdate);  Serial.print("/"); //data
      Serial.print(vmes);   Serial.print("/"); //mes
      Serial.println(vano);                    //ano
      //second,min,hour,day,date,month,year
      setDS3231time(vsec,vmin,vhr,vday,vdate,vmes,vano);
            inData = "";
       }
      // clear the string for new input:;
    //}
    }


}

void blank(){
        for (int k=0;k<N_DEV;k++)
        lc1.clearDisplay(k);
  }


void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
