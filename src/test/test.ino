#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BH1750.h>
#include <EEPROM.h>
#include <avr/sleep.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

BH1750 lightMeter;

#define DomeMultiplier          1.
#define MeteringButtonPin       2
#define PlusButtonPin           3
#define MinusButtonPin          4
#define ModeButtonPin           7   // 5
#define MenuButtonPin           6
#define MeteringModeButtonPin   5   // 7
//#define PowerButtonPin          2

#define MaxISOIndex 57
#define MaxApertureIndex 70
#define MaxTimeIndex 80
#define MaxNDIndex 13
#define MaxFlashMeteringTime 5000 // ms

float lux;
boolean Overflow = 0;
float ISOND;
boolean ISOmode = 0;
boolean NDmode;

boolean PlusButtonState;
boolean MinusButtonState;
boolean MeteringButtonState;
boolean ModeButtonState;
boolean MenuButtonState;
boolean MeteringModeButtonState;

boolean ISOMenu = false;
boolean NDMenu = false;
boolean mainScreen = false;

#define ISOIndexAddr 1
#define apertureIndexAddr 2
#define modeIndexAddr 3
#define T_expIndexAddr 4
#define meteringModeAddr 5
#define ndIndexAddr 6

#define defaultApertureIndex 12
#define defaultISOIndex 11
#define defaultModeIndex 0
#define defaultT_expIndex 19

uint8_t ISOIndex = EEPROM.read(ISOIndexAddr);
uint8_t apertureIndex = EEPROM.read(apertureIndexAddr);
uint8_t T_expIndex = EEPROM.read(T_expIndexAddr);
uint8_t modeIndex = EEPROM.read(modeIndexAddr);
uint8_t meteringMode = EEPROM.read(meteringModeAddr);
uint8_t ndIndex = EEPROM.read(ndIndexAddr);

int battVolts;
#define batteryInterval 10000
double lastBatteryTime = 0;

#include "test.h"


void setup() {
    pinMode(PlusButtonPin, INPUT_PULLUP);
    pinMode(MinusButtonPin, INPUT_PULLUP);
    pinMode(MeteringButtonPin, INPUT_PULLUP);
    pinMode(ModeButtonPin, INPUT_PULLUP);
    pinMode(MenuButtonPin, INPUT_PULLUP);
    pinMode(MeteringModeButtonPin, INPUT_PULLUP);

    battVolts = getBandgap();

    Wire.begin();
    lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE_2);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextColor(WHITE);
    display.clearDisplay();

    if (apertureIndex > MaxApertureIndex) {
      apertureIndex = defaultApertureIndex;
    }

    if (ISOIndex > MaxISOIndex) {
    ISOIndex = defaultISOIndex;
    }

    if (T_expIndex > MaxTimeIndex) {
      T_expIndex = defaultT_expIndex;
    }

    if (modeIndex < 0 || modeIndex > 1) {
      // Aperture priority. Calculating shutter speed.
      modeIndex = 0;
    }

    if (meteringMode > 1) {
      meteringMode = 0;
    }

    if (ndIndex > MaxNDIndex) {
      ndIndex = 0;
    }

    lux = getLux();
    refresh();
}


void loop() {
  if (millis() >= lastBatteryTime + batteryInterval) {
    lastBatteryTime = millis();
    battVolts = getBandgap();
  }

  readButtons();
  
  menu();

  if (MeteringButtonState == 0) {
    SaveSettings();

    lux = 0;
    refresh();

    if (meteringMode == 0) {
      lightMeter.configure(BH1750::ONE_TIME_HIGH_RES_MODE_2);

      lux = getLux();

      if (Overflow == 1) {
        delay(10);
        getLux();
      }

      refresh();
      delay(200);
    } else if (meteringMode == 1) {
      lightMeter.configure(BH1750::CONTINUOUS_LOW_RES_MODE);

      unsigned long startTime = millis();
      uint16_t currentLux = 0;
      lux = 0;

      while (true) {
        if (startTime + MaxFlashMeteringTime < millis()) {
          break;
        }

        currentLux = getLux();
        delay(16);

        if (currentLux > lux) {
          lux = currentLux;
        }
      }

      refresh();
    }
  }
}
