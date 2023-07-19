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
// Heartrate Sensor analog input
const int pinAdc = A0;
//Smoothing/averaging of the signal
const int numReadings = 600;

int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int average = 0;


void setup() {
  // initialize the serial communication:
    //reset array for smoothing
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  Serial.begin(9600);
  pinMode(11, INPUT); // Setup for leads off detection LO +
  pinMode(12, INPUT); // Setup for leads off detection LO -

}

void loop() {


  if((digitalRead(11) == 1)||(digitalRead(12) == 1)){
    Serial.println('0');
  }
  else{
    // send the value of analog input 0:
      Serial.println(analogRead(pinAdc));
  //     // subtract the last reading:
  //     total = total - readings[readIndex];
  //     // read from the sensor:
  //     readings[readIndex] = analogRead(pinAdc);
  //     ;
  //     // add the reading to the total:
  //     total = total + readings[readIndex];
  //     // advance to the next position in the array:
  //     readIndex = readIndex + 1;

  //     // if we're at the end of the array...
  //     if (readIndex >= numReadings) {
  //       // ...wrap around to the beginning:
  //       readIndex = 0;
  //     }

  //     // calculate the average:
  //     average = total / numReadings;
  //     // send it to the computer as ASCII digits
  //     Serial.println("average reading of analog sensor: ");
  //     Serial.println(average);
  }
  //Wait for a bit to keep serial data from saturating
  delay(1);
}