/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  See README for complete attributions.
*/

#include <G35String.h>
#include <StockPrograms.h>

// Total # of lights on string (usually 50, 48, or 36). Maximum is 63, because
// the protocol uses 6-bit addressing and bulb #63 is reserved for broadcast
// messages.
#define LIGHT_COUNT (50)

// Arduino pin number. Pin 13 will blink the on-board LED.
#define G35_PIN (9)

G35String lights(G35_PIN, LIGHT_COUNT);
LightProgram* programs[] = { new ChasingWhiteRedBlue(lights) };

void setup() {
  lights.enumerate();
  programs[0]->Init();
  //lights.do_test_patterns();
}

void loop() {
  delay(programs[0]->Do());
  delay(300);
}
