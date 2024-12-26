// Wire Controller Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI peripheral device
// Refer to the "Wire Peripheral Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <Arduino.h>
#include <Wire.h>

void setup() {
    Wire.begin();        // join i2c bus (address optional for master)
    Serial.begin(9600);  // start serial for output
}

void loop() {
    // Serial.println("Instruction with no data:");
    // Wire.beginTransmission(0x10);
    // Wire.write(0x10);
    // Wire.endTransmission();
    // Wire.requestFrom(0x10, 1);
    // get();
    // Serial.println("Instruction with data (0x7F):");
    // Wire.beginTransmission(0x10);
    // Wire.write(0x10);
    // Wire.write(0x7F);
    // Wire.endTransmission();
    // Wire.requestFrom(0x10, 2);
    // get();
    // get();
    // Serial.println("Instruction with data (0xFF):");
    // Wire.beginTransmission(0x10);
    // Wire.write(0x10);
    // Wire.write(0xFF);
    // Wire.endTransmission();
    // Wire.requestFrom(0x10, 2);
    // get();
    // get();
    Wire.beginTransmission(0x10);
    Wire.write(0xDE);
    // Wire.write(0xAD);
    // Wire.write(0xBE);
    Wire.endTransmission();
    Wire.requestFrom(0x10, 4);
    Serial.println(Wire.read(), HEX);
    Serial.println(Wire.read(), HEX);
    Serial.println(Wire.read(), HEX);
    Serial.println(Wire.read(), HEX);

    delay(1000);
}