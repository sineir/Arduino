/*
>esp8266
>moc + bta
*/

#include <ESP8266WiFi.h>
#include <MQTT.h>

#define AP_SSID     "you ap ssid"
#define AP_PASSWORD "you password"  

#define EIOTCLOUD_USERNAME "you user"
#define EIOTCLOUD_PASSWORD "yuo password"

// create MQTT object
#define EIOT_CLOUD_ADDRESS "cloud.iot-playground.com"

#define DO_TOPIC        "/Sensor.Parameter1"

#define PIN_DO_1        2  // Num.Pin.Module pin1 

#define MODULE_ID_1     5  // numero de ordem device no servidor


#define PIN_BTN         0  // botao on off relay Pin.0.Module
#define relay_on        0  // relay on
#define relay_off       1  // relay off
String relay01_in = ""; // subscribe.. leitura no servidor....


MQTT myMqtt("", EIOT_CLOUD_ADDRESS, 1883);


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);  
  WiFi.begin(AP_SSID, AP_PASSWORD);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(AP_SSID);
    
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  };

  Serial.println("WiFi connected");
  Serial.println("Connecting to MQTT server");  

  String clientName;
  uint8_t mac[6];
  WiFi.macAddress(mac);
  clientName += macToStr(mac);
  clientName += "-";
  clientName += String(micros() & 0xff, 16);
  myMqtt.setClientId((char*) clientName.c_str());

  Serial.print("MQTT client id:");
  Serial.println(clientName);

  // setup callbacks
  myMqtt.onConnected(myConnectedCb);
  myMqtt.onDisconnected(myDisconnectedCb);
  myMqtt.onPublished(myPublishedCb);
  myMqtt.onData(myDataCb);
  
  //////Serial.println("connect mqtt...");
  myMqtt.setUserPwd(EIOTCLOUD_USERNAME, EIOTCLOUD_PASSWORD);  
  myMqtt.connect();

  delay(500);

  pinMode(PIN_DO_1, OUTPUT); 
  pinMode(PIN_BTN, INPUT_PULLUP); 

  subscribe();

}

void loop() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  if (!digitalRead(PIN_BTN)   ) {
    Serial.println(" BOTAO PRESSIONADO "); // debug
    if (relay01_in =="1") {
    digitalWrite(PIN_DO_1,relay_off);
    //myMqtt.publish("/" + String(MODULE_ID_1) + DO_TOPIC,"0",1,0);
    relay01_in="0";
    }else{
    relay01_in="1";
    digitalWrite(PIN_DO_1, relay_on);
    }
    myMqtt.publish("/" + String(MODULE_ID_1) + DO_TOPIC,relay01_in,1,0);
    while ( !digitalRead(PIN_BTN) ) delay(100);
  }
}


String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}


void subscribe()
{
    myMqtt.subscribe("/" + String(MODULE_ID_1) + DO_TOPIC); //DO 1
//    myMqtt.subscribe("/" + String(MODULE_ID_2) + DO_TOPIC); //DO 2
}


void myConnectedCb() {
  Serial.println("connected to MQTT server");
  subscribe();
}

void myDisconnectedCb() {
  Serial.println("disconnected. try to reconnect...");
  delay(500);
  myMqtt.connect();
}

void myPublishedCb() {
  Serial.println("published.");
}

void myDataCb(String& topic, String& data) {  
  if (topic == String("/"+String(MODULE_ID_1)+ DO_TOPIC))
  {
    if (data == String("1")){
      digitalWrite(PIN_DO_1, relay_on);
      //myMqtt.publish("/"+String(MODULE_ID_2)+ "/Sensor.Parameter1","MT_DIGITAL_OUTPUT",0,0); // topico,tipo,qos,valor
    }else{
      digitalWrite(PIN_DO_1,relay_off);
      //myMqtt.publish("/"+String(MODULE_ID_2)+ "/Sensor.Parameter1","MT_DIGITAL_OUTPUT",0,1);
    }
    Serial.print("Do 1:");
    Serial.println(data);
    relay01_in = data;
    //topic  = "/" + String(storage.moduleId) + "/ModuleType";
    //myMqtt.publish("/"+String(MODULE_ID_2)+ "/Sensor.Parameter1","MT_DIGITAL_OUTPUT",0,1);
  }
 
}
