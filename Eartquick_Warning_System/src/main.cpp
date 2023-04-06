#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

float i = 0;
int tiltSensor = 16;
int speaker = 0;

int buttonLevel = 14;
int buttonSensor = 2;

int ledR = 13;
int ledG = 15;
int ledB = 12;
int ledAlarm = 3;

int numberOfAttempsMPU = 20;
int mpuFound = 0;

void setRGB(int R, int G, int B)
{
  int rValue = 255 - R;
  int gValue = 255 - G;
  int bValue = 255 - B;
  analogWrite(ledR, rValue);
  analogWrite(ledG, gValue);
  analogWrite(ledB, bValue);
}

void redAlarm(int value)
{
  analogWrite(ledAlarm, value);
}

void playWarnig(int loop_count)
{
  for (int l = 0; l < loop_count; l++)
  {
    redAlarm(255);
    tone(speaker, 349);
    delay(500);
    redAlarm(0);
    noTone(speaker);
  }
}

void playSiren(int loop_count)
{

  for (int l = 0; l < loop_count; l++)
  {
    for (int i = 300; i < 530; i++)
    {
      tone(speaker, i);
      redAlarm((i % 5 == 0));
      delay(20);
    }
    for (int i = 530; i > 300; i--)
    {
      tone(speaker, i);
      redAlarm((i % 5 == 0));
      delay(20);
    }
  }
  redAlarm(0);
  noTone(speaker);
}

void eartquik()
{
  setRGB(255, 0, 0);
  playSiren(1);
}

void shock()
{
  playWarnig(1);
}

void switchSecondarySensor()
{
  setRGB(200, 150, 0);
}

void setup()
{
  pinMode(tiltSensor, INPUT);
  pinMode(ledAlarm, OUTPUT);

  setRGB(0, 0, 0);
  redAlarm(0);
  noTone(speaker);

  Serial.begin(115200);

  while (!Serial)
    delay(10);
  Serial.println("setup");

  Serial.println("MPU6050 test!");
  if (!mpu.begin() && numberOfAttempsMPU > 0)
  {
    delay(10);
    numberOfAttempsMPU--;
  }

  if (mpu.begin())
  {
    mpuFound = 1;
    mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
    mpu.setMotionDetectionThreshold(1);
    mpu.setMotionDetectionDuration(20);
    mpu.setInterruptPinLatch(true);
    mpu.setInterruptPinPolarity(true);
    mpu.setMotionInterrupt(true);
  }
  else
  {
    Serial.println("Failed to find MPU6050 chip");
  }

  delay(100);
}

void loop()
{
  i += 0.1;
  /*  if (digitalRead(tiltSensor) == HIGH)
      shock();*/

  // if (mpu.getMotionInterruptStatus())

  // Print log
  Serial.print("loop");
  Serial.println(i);

  if (mpuFound)
  {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    Serial.print(">AX:");
    Serial.println(a.acceleration.x);
    Serial.print(">AY:");
    Serial.println(a.acceleration.y);
    Serial.print(">AZ:");
    Serial.println(a.acceleration.z);
    Serial.print(">GX:");
    Serial.println(g.gyro.x);
    Serial.print(">GY:");
    Serial.println(g.gyro.y);
    Serial.print(">GZ:");
    Serial.println(g.gyro.z);
  }

  delay(100);
}
