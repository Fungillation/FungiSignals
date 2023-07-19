/******************************************************************************
Heart_Rate_Display.ino
Demo Program for AD8232 Heart Rate sensor.
Casey Kuhns @ SparkFun Electronics
6/27/2014
https://github.com/sparkfun/AD8232_Heart_Rate_Monitor

The AD8232 Heart Rate sensor is a low cost EKG/ECG sensor.  This example shows
how to create an ECG with real time display.  The display is using Processing.
This sketch is based heavily on the Graphing Tutorial provided in the Arduino
IDE. http://www.arduino.cc/en/Tutorial/Graph

Resources:
This program requires a Processing sketch to view the data in real time.

Development environment specifics:
	IDE: Arduino 1.0.5
	Hardware Platform: Arduino Pro 3.3V/8MHz
	AD8232 Heart Monitor Version: 1.0

This code is beerware. If you see me (or any other SparkFun employee) at the
local pub, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

//Reihenfolge der Serialplots:
/*
1. humidity
2. temperature in c
3. visibility
4. infrared
5. mushroom readings
*/
// Sunlight sensor
#include "SI114X.h"
SI114X SI1145 = SI114X();

//Humidity and Temp Sensor
#include "DHT.h"
#define DHTTYPE DHT11  // DHT 11

#define DHTPIN 2           // what pin we're connected to（DHT10 and DHT20 don't need define it）
DHT dht(DHTPIN, DHTTYPE);  //   DHT11 DHT21 DHT22


// Heartrate Sensor analog input
const int pinAdc = A0;
//Smoothing/averaging of the signal
const int numReadings = 600;

int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int average = 0;


// ...

float initialHumidity = 0.0;
float initialTemperature = 0.0;
unsigned long lastTempHumReadTime = 0;
const unsigned long tempHumReadInterval = 60000; // 10 seconds

void setup() {
  // ...
    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  Serial.begin(115200);
  // humidity and temp
  dht.begin();
  delay(1000);
  // Read initial humidity and temperature values
  initialHumidity = dht.readHumidity();
  initialTemperature = dht.readTemperature();
  lastTempHumReadTime = millis();
}

void loop() {
  // ...

  unsigned long currentMillis = millis();

  // Read humidity and temperature every 10 seconds
  if (currentMillis - lastTempHumReadTime >= tempHumReadInterval) {
    lastTempHumReadTime = currentMillis;
    //Serial.println("reading");
    initialHumidity = dht.readHumidity();
    initialTemperature = dht.readTemperature();
    // float temp_hum_val[2];
    // if (dht.readTempAndHumidity(temp_hum_val)) {
    //   Serial.println(temp_hum_val[0]);
    //   Serial.println(temp_hum_val[1]);
    //   initialHumidity = temp_hum_val[0];
    //   initialTemperature = temp_hum_val[1];
    // }
  }

  // ...

  float humidity = initialHumidity;
  float temperature = initialTemperature;
  int visibility = SI1145.ReadVisible();
  int infrared = SI1145.ReadIR();
  float uv = (float)SI1145.ReadUV() / 100;
  int mushroomReadings = analogRead(pinAdc);

  // ...

char buffer[100];
// Format humidity using dtostrf()
dtostrf(humidity, 5, 2, buffer); // Format: 5 characters with 2 decimal places
strcat(buffer, ",");

// Format temperature using dtostrf() and concatenate with buffer
char temp[20]; // Temporary buffer for temperature
dtostrf(temperature, 5, 2, temp); // Format: 5 characters with 2 decimal places
strcat(buffer, temp);
strcat(buffer, ",");

// Format visibility using sprintf() and concatenate with buffer
sprintf(temp, "%d,", visibility);
strcat(buffer, temp);

// Format infrared using sprintf() and concatenate with buffer
sprintf(temp, "%d,", infrared);
strcat(buffer, temp);

// Format uv using dtostrf() and concatenate with buffer
dtostrf(uv, 3, 2, temp); // Format: 3 characters with 2 decimal places
strcat(buffer, temp);
strcat(buffer, ",");

// Format mushroomReadings using sprintf() and concatenate with buffer
sprintf(temp, "%d", mushroomReadings);
strcat(buffer, temp);

// Print the formatted string
Serial.println(buffer);
delay(1);
}

  
