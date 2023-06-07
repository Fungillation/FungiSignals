/* this is a test for the grove ST114x sunlight and the grove humidity and temperature sensor */

#include <Wire.h>

#include "Arduino.h"

// Sunlight sensor
#include "SI114X.h"
SI114X SI1145 = SI114X();

//Humidity and Temp Sensor stuff
#include "DHT.h"
#define DHTTYPE DHT11  // DHT 11

#define DHTPIN 2           // what pin we're connected to（DHT10 and DHT20 don't need define it）
DHT dht(DHTPIN, DHTTYPE);  //   DHT11 DHT21 DHT22

// Sound Sensor Setup for signal smoothing test
const int pinAdc = A0;

//Smoothing/averaging of the signal
const int numReadings = 10;

int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int average = 0;

void setup() {

  //reset array for smoothing
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  Serial.begin(115200);

  Serial.println("Beginning Si1145 and DHT11");
  //humidity and temp
  dht.begin();
  //Sunlight
  while (!SI1145.Begin()) {
    Serial.println("Si1145 is not ready!");
    delay(1000);
  }
  Serial.println("Si1145 is ready!");
}



// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor
void loop() {
  //Temp and humidity

  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(pinAdc);
  ;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  Serial.println("average reading of analog sensor: ");
  Serial.print(average);
  //Serial.println(analogRead(pinAdc));


  float temp_hum_val[2] = { 0 };


  if (!dht.readTempAndHumidity(temp_hum_val)) {
    Serial.print("Humidity: ");
    Serial.println(temp_hum_val[0]);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.println(temp_hum_val[1]);
    Serial.print(" *C");
  } else {
    Serial.println("Failed to get temprature and humidity value.");
  }


  Serial.print("//--------------------------------------//\r\n");
  Serial.print("Vis: ");
  Serial.println(SI1145.ReadVisible());
  Serial.print("IR: ");
  Serial.println(SI1145.ReadIR());
  //the real UV value must be div 100 from the reg value , datasheet for more information.
  Serial.print("UV: ");
  Serial.println((float)SI1145.ReadUV() / 100);
  delay(1);
}
