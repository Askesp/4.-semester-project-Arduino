#include <OneWire.h>
#include <Servo.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 3

const int button1 = 2; // This button is for profile Aske 
const int button2 = 4; // This button is for profile Emil 
const int button3 = 7; // This button is for profile Rasmus 
const int button4 = 8; // This button is for profile Jacob
const int pump1 = 6; // this pump is a dc 12v water pump for cold water
const int pump2 = 11; // this pump is a dc 12v water pump for hot water 
Servo myservo; // create servo object to control a servo
const float targetTemp1 = 37.0;
const float targetTemp2 = 33.0;
const float targetTemp3 = 35.0;
const float targetTemp4 = 38.0;
const float coldTemp = 5.0;
const float hotTemp = 50.0;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;


void setup(void)
{ pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  myservo.attach(12);  

  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();
  sensors.setResolution(insideThermometer, 9);
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();
}


void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  if(tempC == DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Error: Could not read temperature data");
    return;
  }
  Serial.print("Temp C: ");
  Serial.print(tempC);
}

void loop(void)
{ 
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); 
  Serial.println("DONE");
  printTemperature(insideThermometer); 
  delay(1000);

float currentTemp = sensors.getTempC(insideThermometer);
  float targetTemp;
  if (digitalRead(button1) == LOW) {
    targetTemp = targetTemp1;
    myservo.write(25);
    Serial.println("Button 1");
  } else if (digitalRead(button2) == LOW) {
    targetTemp = targetTemp2;
    myservo.write(90);
    Serial.println("Button 2");
  } else if (digitalRead(button3) == LOW) {
    targetTemp = targetTemp3;
    myservo.write(135);
    Serial.println("Button 3");
  } else if (digitalRead(button4) == LOW) {
    targetTemp = targetTemp4;
    myservo.write(180);
    Serial.println("Button 4");
  } else {
    analogWrite(pump1, LOW);
    analogWrite(pump2, LOW);
    Serial.println("    It's working");
    myservo.write(0);
    return;
  }

  float ratio = (targetTemp - coldTemp) / (hotTemp - coldTemp);
  int hotSpeed = ratio * 255;
  int coldSpeed = (1 - ratio) * 255;
  hotSpeed = map(hotSpeed, 1, 255, 80, 255);
  coldSpeed = map(coldSpeed, 1, 255, 80, 255);
  analogWrite(pump1, coldSpeed);
  analogWrite(pump2, hotSpeed);
  Serial.println(coldSpeed);
  Serial.println(hotSpeed);
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
   
}