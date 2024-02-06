#pragma once

#include "Arduino.h"

// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>        // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy: #include "SSD1306.h"

#define OLED_SDA 5
#define OLED_SCL 4

#define DISPLAY_LINE_1 0, 0
#define DISPLAY_LINE_2 0, 16
#define DISPLAY_LINE_3 0, 32
#define DISPLAY_LINE_4 0, 48

extern SSD1306Wire display;

void displayIntArray(const char* prefix, int *array, size_t offset, size_t length);