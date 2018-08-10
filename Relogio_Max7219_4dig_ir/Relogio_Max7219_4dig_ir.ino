#include "LedControl.h"
#include "Wire.h"
//https://github.com/z3t0/Arduino-IRremote
#include <IRremote.h>
int RECV_PIN = 9;
IRrecv irrecv(RECV_PIN);
decode_results results;

#define DS3231_I2C_ADDRESS 0x68
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

void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
  }

  byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
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
//int uni,dez,cen,mil;
int ctr=0;
byte brilho=0;
int funcao=0;
int temp = 0;

void setup() {
    int addr=0,i;
    Wire.begin();
    irrecv.enableIRIn(); // Start the receiver
  // put your setup code here, to run once:
   //wake up the MAX72XX from power-saving mode 
   lc1.shutdown(0,false);
   //set a medium brightness for the Leds
  //i2c_eeprom_write_byte(0x57,0x00,0x08);
  brilho = i2c_eeprom_read_byte(0x57, 0x00); //access an address from the memory
  lc1.setIntensity(0x00,brilho);
//   mil = 9; cen=8;
   //second,min,hour,day,date,month,year
   //setDS3231time(0,15,23,7,12,8,17);
              //set sqw 1hz  
              Wire.beginTransmission(DS3231_I2C_ADDRESS);
              Wire.write(0x0E); // set next input to start at the seconds register
              Wire.write(0x00); // set hours
              Wire.endTransmission();
}

void loop() {
 byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,&year);

if (irrecv.decode(&results))
    {
    // Serial.println(results.value, HEX);
    //if (results.value == 16754775 ) brilho--;
    //if (results.value == 16748655 ) brilho++;
    lc1.setIntensity(0,brilho);

    if (results.value == 16736925 ) { // mode
     lc1.clearDisplay(0);

      funcao++;

      if (funcao==3){
      lc1.setLed(0,3,3,true); 
      lc1.setLed(0,3,4,true); 
      lc1.setLed(0,3,5,true); 
      lc1.setLed(0,3,6,true); 
      lc1.setLed(0,3,7,true); 
      }

      if (funcao==2){
      lc1.setLed(0,3,1,true); 
      lc1.setLed(0,3,2,true); 
      lc1.setLed(0,3,3,true); 
      lc1.setLed(0,3,5,true); 
      lc1.setLed(0,3,6,true); 
      }
      
      if (funcao==1){
      lc1.setLed(0,3,2,true); 
      lc1.setLed(0,3,3,true); 
      lc1.setLed(0,3,5,true); 
      lc1.setLed(0,3,6,true); 
      lc1.setLed(0,3,7,true); 
      }
      
      }
    if (results.value == 16754775 ) { //vol -
        switch (funcao) {
          case 1:
            if (hour==0) {
            temp=23;
            }else{
            temp=hour-1;
            }
              Wire.beginTransmission(DS3231_I2C_ADDRESS);
              Wire.write(2); // set next input to start at the seconds register
              Wire.write(decToBcd(temp)); // set hours
              Wire.endTransmission();
            break;
          case 2:
            if (minute==0){
              temp=59;
            }else{
            temp=minute-1;
            }
              Wire.beginTransmission(DS3231_I2C_ADDRESS);
              Wire.write(1); // set next input to start at the seconds register
              Wire.write(decToBcd(temp)); // set hours
              Wire.endTransmission();
            break;
          case 0:
          case 4:
          if (brilho==0) brilho=1;
          brilho--;
            i2c_eeprom_write_byte(0x57,0x00,brilho);
            break;
          }
      }

    if (results.value == 16748655 ) { //vol +
        switch (funcao) {
          case 1:
            temp=hour+1;
            if (temp==24) temp=0;
              Wire.beginTransmission(DS3231_I2C_ADDRESS);
              Wire.write(2); // set next input to start at the seconds register
              Wire.write(decToBcd(temp)); // set hours
              Wire.endTransmission();              
            break;
          case 2:
            temp=minute+1;
            if (temp==60) temp=0;
              Wire.beginTransmission(DS3231_I2C_ADDRESS);
              Wire.write(1); // set next input to start at the seconds register
              Wire.write(decToBcd(temp)); // set hours
              Wire.endTransmission();              
            break;
          case 0:
          case 4:
            brilho++;
            if (brilho==16) brilho=15;
            i2c_eeprom_write_byte(0x57,0x00,brilho);

            break;
          }
      }
    
     irrecv.resume(); // Receive the next value
    }

  if (funcao ==1) {
      lc1.setDigit(0, 0, hour%10, false);
      lc1.setDigit(0, 1, hour/10, false);
    }
  
  if (funcao ==2) {
      lc1.setDigit(0, 0, minute%10, false);
      lc1.setDigit(0, 1, minute/10, false);
    }

  if (funcao ==4) {
      lc1.setDigit(0, 0, brilho%10, false);
      lc1.setDigit(0, 1, brilho/10, false);
      }
  if (funcao ==5) funcao=0;
  
  if (funcao == 3) {
              Wire.beginTransmission(DS3231_I2C_ADDRESS);
              Wire.write(0); // set next input to start at the seconds register
              Wire.write(decToBcd(10)); // set hours
              Wire.endTransmission();              
              funcao=4;
    }
  
  //month=5; dayOfMonth=30;  year=17;  second=30;  ctr=0;
//lc1.setIntensity(0,second%15);
if (funcao == 0 ) {
  
  if (second == 30 && ctr == 0 ){

      lc1.setDigit(0, 0, month%10, false);
      lc1.setDigit(0, 1, month/10, false);
      lc1.setDigit(0, 2, dayOfMonth%10, false);
      lc1.setDigit(0, 3, dayOfMonth/10, false);
      ctr = 1 ;
      lc1.setDigit(0, 4, year%10, false);
      lc1.setDigit(0, 5, year/10, false);
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
       //lc1.setDigit(0, 3, 15, false);
       lc1.setRow(0,3,0x00);
       }
      lc1.setDigit(0, 4, second%10, false);
      lc1.setDigit(0, 5, second/10, false);
        delay(250);
    /*
    if (hour>18 and hour < 6){
      lc1.setIntensity(0,8);
    }else{
      lc1.setIntensity(0,14);
    }
*/
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
