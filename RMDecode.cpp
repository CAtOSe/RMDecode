/*
  RMDecode.cpp - Library for decoding RoboMote signals.
  Created by Paulius Jasėnas, October 19, 2019.
  Released under GNU General Public License v3.0
*/

#include "Arduino.h"
#include "RMDecode.h"

byte _nec_state = 0;
unsigned long _nec_code = 0;
bool _nec_ok = 0;
byte _i = 0;
long _overflow_count = 0;

#include "RM_RemoteRead.h"


RMDecode::RMDecode() {
  attachInterrupt(IR_INT, remote_read, CHANGE);
}

void RMDecode::begin() {
  timeoutTimer = millis();
  varsC = 0;
  strat = 0;
  loadedStrat = false;
  loadedEvent = false;

  _nec_state = 0;
  _nec_code = 0;
  _nec_ok = 0;
  _i = 0;
  _overflow_count = 0;
  msgs = 0;

  // Timer setup
  TCCR2A = 0; // Set mode to Normal
  TCCR2B = 0; // Disable Timer2
  TCNT2 = 0; // Set value to 0
  TIMSK2 = 1; // Enable overflow vector

  // Enable external interrupt
  attachInterrupt(IR_INT, remote_read, CHANGE);
}

void RMDecode::stop() {
  detachInterrupt(IR_INT);
  _nec_ok = false;
  _nec_state = 0;

  TCCR2B = 0; // Disable Timer2
  TCNT2 = 0; // Set value to 0
}

uint8_t RMDecode::decode(uint8_t mode) {
  if (getMessage()) {
    bool receivedStrat = false;
    bool receivedEvent = false;

    if (address == necStart && bitRead(mode, 0)) {
      Serial.println("Start");
      msgs = command - 1;
      timeoutTimer = millis() + MSG_TIMEOUT;
      uint8_t msgState = 0;
      uint8_t m = 0;
      varsC = 0;
      strat = 0;
      bool waitingForVar = false;

      while (timeoutTimer > millis() && m < msgs) {
        if (getMessage()) {
          m++;
          if (address == necStrategy) {
            Serial.println("STRAT");
            strat = command;
          } else if (address == necEnd) {
            Serial.println("End");
            receivedStrat = true;
            loadedStrat = true;
            break;
          } else if (address == necVar) {
            Serial.println("V1");
            waitingForVar = true;
          } else if (waitingForVar) {
            Serial.println("V2");
            uint32_t var = ((uint32_t) address << 16) | ((uint32_t) command << 8) | (inv_command);
            memcpy(&vars[varsC], &var, sizeof(uint32_t));
            varsC++;
            waitingForVar = false;
          }
        }
      }
    } else if (address == necEvent && bitRead(mode, 1)) {
      event = command;
      receivedEvent = true;
      loadedEvent = true;
    } else {
      Serial.println(address, HEX);
    }
    if (receivedStrat) return 1;
    if (receivedEvent) return 2;
  } else return false;
}

float RMDecode::getFloat(uint8_t index) {
  float temp;
  memcpy(&temp, &vars[index], sizeof(uint32_t));
  return temp;
}

bool RMDecode::getBool(uint8_t index) {
  return vars[index];
}

long RMDecode::getInt(uint8_t index) {
  return vars[index];
}

uint32_t RMDecode::getRaw(uint8_t index) {
  return vars[index];
}

uint8_t RMDecode::getVarCount() {
  return varsC;
}

bool RMDecode::getMessage() {
  if (_nec_ok) {
    attachInterrupt(IR_INT, remote_read, CHANGE);
    _nec_state = 0;
    _nec_ok = 0;
    _i = 0;
    _overflow_count = 0;

    // Reset timer
    TCCR2B = 0; // Disable Timer2
    TCNT2 = 0; // Set value to 0

    event = 0; // Should I reset it? It works, so I wont touch this

    address = _nec_code >> 16;
    command = _nec_code >> 8;
    inv_command = _nec_code;
    return true;
  } else return false;
}

uint8_t RMDecode::getStrategy() {
  return strat;
}

bool RMDecode::isEvent() {
  return loadedEvent;
}

bool RMDecode::isStrategy() {
  return loadedStrat;
}

uint8_t RMDecode::getEvent() {
  loadedEvent = false;
  return event;
}

ISR(TIMER2_OVF_vect) {
  if(_overflow_count > 10) {
    // Reset decoding variables
    _nec_state = 0;
    _i = 0;
    _overflow_count = 0;

    // Reset timer
    TCCR2A = 0; // Set mode to Normal
    TCCR2B = 0; // Disable Timer2
    TCNT2 = 0; // Set value to 0
    TIMSK2 = 1; // Enable overflow vector

    Serial.println("Too many overflows");
  } else {
    _overflow_count++;
    TCNT2 = 0;
  }
  //_nec_state = 0;                                 // Reset decoding process
  //TCCR1B = 0;                                    // Disable Timer1 module

}
