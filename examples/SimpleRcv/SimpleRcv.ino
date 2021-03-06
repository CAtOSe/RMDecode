#include "RMDecode.h" // Includes the library *duh*

RMDecode robomote(3); // Declare remote object

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("Hello");
  robomote.begin(); // Initialize the IR receiver, timer and interrupt.

}

void loop() {
  // Call robomote.decode() in a loop to check for IR signals
  // Returns true if a signal is found
  uint8_t result = robomote.decode(3); // Setting mode to 3. Strategy and event mode.
  if (result == 1) {
    Serial.print("Startegy: ");
    Serial.println(robomote.getStrategy()); // Call this to get selected strategy

    Serial.print("V1: ");
    Serial.println(robomote.getInt(0)); // Call this to get integer (long) at index 0
    // NOTE: nothing prevents you from getting this variable as a float, but you're gonna get a bad value. So be aware of the types.

    Serial.print("V2: ");
    Serial.println(robomote.getFloat(1), 6);

    Serial.print("V3: ");
    Serial.println(robomote.getInt(2));

    Serial.print("V4: ");
    Serial.println(robomote.getFloat(3), 6);

    Serial.print("V4: ");
    Serial.println(robomote.getBool(4));

  } else if (result == 2) {
    Serial.print("Event: ");
    Serial.println(robomote.getEvent());
  }

  //robomote.stop(); // Stop receiving signals and dissable interrupt.
  //while(1); // Halt program
}
