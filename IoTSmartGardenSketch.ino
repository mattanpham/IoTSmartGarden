//include this for time https://techzeero.com/arduino-tutorials/arduino-time-without-rtc-module/#Download-Arduino-Time-Library

//include this for dht https://www.circuitbasics.com/how-to-set-up-the-dht11-humidity-sensor-on-an-arduino/
#include <dht.h>
#include <SoftwareSerial.h>
dht DHT;
SoftwareSerial hc06(0,1);
#define DHT11_PIN 7
#define LDR_PIN A0
#define MS_PIN A1
#define RS_PIN A2
#define relayOFF LOW
#define relayON HIGH
#define TempC DHT.temperature

const int RELAY_PIN1 = 8;
const int RELAY_PIN2 = 12;
const int RELAY_PIN3 = 13;
const int MOISTURE_LEVEL = 300;
char Incoming_Value = 0;
void setup() {
  //setups the relay zones
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(RELAY_PIN3, OUTPUT);
  //starts the relays as off
  digitalWrite(RELAY_PIN1, relayOFF);
  digitalWrite(RELAY_PIN2, relayOFF); 
  digitalWrite(RELAY_PIN3, relayOFF);   
  Serial.begin(9600);
  hc06.begin(9600);

}

void loop() {
  //Bluetooth Code
  if (hc06.available() > 0) {
    Incoming_Value = hc06.read();
    Serial.print(Incoming_Value);
    if (Incoming_Value == '1') {
      digitalWrite(RELAY_PIN1, relayON);
      delay(30000); 
    }
    else if (Incoming_Value == '2') {
      digitalWrite(RELAY_PIN2, relayON);
      delay(30000); 
    }
    else if(Incoming_Value == '3') {
      digitalWrite(RELAY_PIN3, relayON); 
      delay(30000); 
    }
    else if(Incoming_Value == '4') {
      digitalWrite(RELAY_PIN1, relayOFF);
    }
    else if(Incoming_Value == '5') {
      digitalWrite(RELAY_PIN2, relayOFF);
    }
    else if(Incoming_Value== '6') {
      digitalWrite(RELAY_PIN3, relayOFF);
    }
  }
  
  
  //DHT Code
  int chk = DHT.read11(DHT11_PIN);

  //******CHECK THIS*******
  float TempF = 0;
  TempF = (TempC*1.8)+32;
  Serial.print("Temperature = ");
  Serial.println(TempF);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  delay(500);

  //LDR Code
  int lightValue = analogRead(LDR_PIN);
  Serial.print("Brightness = ");
  Serial.print(lightValue);
  
  if (lightValue > 900) {
    Serial.println(" - Dark");
  } else if (lightValue > 800) {
    Serial.println(" - Dim");
  } else if ((lightValue < 800) && (lightValue > 500)) {
    Serial.println(" - Light");
  } else if ((lightValue > 200) && (lightValue < 500)) {
    Serial.println(" - Bright");
  } else {
    Serial.println(" - Very bright");
  }
  delay(500);

  //Moisture Sensor Code
  int moistValue = analogRead(MS_PIN); // read the analog value from sensor
  Serial.print("Moisture: ");
  Serial.println(moistValue);
  delay(500);

  //Rain Sensor Code
  int rainValue = analogRead(A2);//read value
  Serial.print("Rain Value: ");
  Serial.println(rainValue);
  if (rainValue < 500) {//check condition
    digitalWrite(4, HIGH);
    Serial.print("Heavy rain  LED on ");
    delay(500);  
  } 
  else {
    digitalWrite(4, LOW);
  }

  
  //****Water Code****
  //if ((TempF > 40) && (rainValue > 500) && (moistValue > MOISTURE_LEVEL) && (lightValue < 500)) {
   // digitalWrite(RELAY_PIN1, relayON);
  //  digitalWrite(RELAY_PIN2, relayON); 
  //  digitalWrite(RELAY_PIN3, relayON); 
  //  delay(10000);
 // }


  //**OVERRIDE**
  if (TempF < 40){
    digitalWrite(RELAY_PIN1, relayOFF);
    digitalWrite(RELAY_PIN2, relayOFF); 
    digitalWrite(RELAY_PIN3, relayOFF);  
  }
  
  //When heavy rain do not water
  if (rainValue < 500) {//check condition
    digitalWrite(RELAY_PIN1, relayOFF);
    digitalWrite(RELAY_PIN2, relayOFF); 
    digitalWrite(RELAY_PIN3, relayOFF);   
  }

  //****REQUIRES TESTING OF MOISTURE LEVELS****
  //decides to water based on moisture level
  if(moistValue < MOISTURE_LEVEL) {
    digitalWrite(RELAY_PIN1, relayOFF);
    digitalWrite(RELAY_PIN2, relayOFF); 
    digitalWrite(RELAY_PIN3, relayOFF); 
  }

  //******REQUIRES TESTING OF LIGHT LEVELS*****
  //if light is dim or lower don't water
  if (lightValue > 500) {
    digitalWrite(RELAY_PIN1, relayOFF);
    digitalWrite(RELAY_PIN2, relayOFF); 
    digitalWrite(RELAY_PIN3, relayOFF); 
  }
}
