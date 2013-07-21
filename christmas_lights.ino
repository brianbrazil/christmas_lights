/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  See README for complete attributions.
*/

#include <G35String.h>
#include <StockPrograms.h>

#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>

#define LIGHT_COUNT (50)

#define G35_PIN (9)
#define BUTTON_PIN (7)

#define BACKLIGHT_MILLIS (10000)
#define BACKLIGHT_ON (0x1)
#define BACKLIGHT_OFF (0x0)

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

G35String lights(G35_PIN, LIGHT_COUNT);
LightProgram* programs[] = { new SteadyWhite(lights),
                             new ChasingWhiteRedBlue(lights),
                             new ChasingMultiColors(lights),
                             new ChasingRainbow(lights),
                             new FadeInFadeOutSolidColors(lights)
                           };

int numPrograms = sizeof(programs) / sizeof(LightProgram*);
int currentProgram = 0;

unsigned long stepTimeout = 0;

int buttonState = LOW;
int previousButtonState = LOW;

boolean backlightOn = true;
unsigned long backlightTimeout = millis() + BACKLIGHT_MILLIS;

void setup() {
  lcd.begin(16, 2);
  displayCurrentProgram();
  pinMode(BUTTON_PIN, INPUT);
  lights.enumerate();
  programs[0]->Init();
}

void loop() {
  if (millis() >= stepTimeout) stepTimeout = step();
  if (millis() >= backlightTimeout && backlightOn) turnBacklightOff();
  processButton();
}

unsigned long step() {
  return millis() + programs[currentProgram]->Do();
}

void processButton() {  
  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH && !backlightOn) {
    turnBacklightOn();
  } else if (buttonState == HIGH && previousButtonState == LOW) {
    currentProgram = (currentProgram+1) % numPrograms;
    programs[currentProgram]->Init();
    displayCurrentProgram();
    turnBacklightOn();
  }
  previousButtonState = buttonState;
  delay(1);
}

void displayCurrentProgram() {
  char programName[40];
//  strcpy(programName, programs[currentProgram]->GetName());
  char* token = strtok(strcpy(programName, programs[currentProgram]->GetName()), " ");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(parseLine(token));
  lcd.setCursor(0,1);
  lcd.print(parseLine(token));
}

void turnBacklightOn() {
  lcd.setBacklight(BACKLIGHT_ON);
  backlightOn = true;
  backlightTimeout = millis() + BACKLIGHT_MILLIS;
}

void turnBacklightOff() {
  lcd.setBacklight(BACKLIGHT_OFF);
  backlightOn = false;
}

String parseLine(char*& token) {
  String line = "";
  while (line.length() + strlen(token) <= 16) {
    line += String(token);
    if (line.length() < 16) line += " ";
    token = strtok(NULL, " ");
  }
  return line;
}
