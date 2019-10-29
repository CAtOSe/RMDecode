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

#include "RM_RemoteRead.h"


RMDecode::RMDecode() {
}

void RMDecode::begin() {
  timeoutTimer = millis();
  varsC = 0;
  strat = 0;
  loadedStrat = false;
  loadedEvent = false;

  _nec_ok = false;
  _nec_state = 0;
  msgs = 0;
  TCCR1A = 0;
  TCCR1B = 0;                                    // Disable Timer1 module
  TCNT1  = 0;                                    // Set Timer1 preload value to 0 (reset)
  TIMSK1 = 1;                                    // enable Timer1 overflow interrupt
  attachInterrupt(IR_INT, remote_read, CHANGE);       // Enable external interrupt (INT0)
}

void RMDecode::stop() {
  detachInterrupt(IR_INT);
  _nec_ok = false;
  _nec_state = 0;
  loadedStrat = false;
  loadedEvent = false;
  TCCR1B = 0;
}

bool RMDecode::decode() {
  if (getMessage()) {
    if (address == necStart) {
      msgs = command - 1;
      timeoutTimer = millis() + MSG_TIMEOUT;
      uint8_t msgState = 0;
      uint8_t m = 0;
      varsC = 0;
      strat = 0;
      loadedStrat = false;
      loadedEvent = false;
      bool waitingForVar = false;

      while (timeoutTimer > millis() && m < msgs) {
        if (getMessage()) {
          m++;
          if (address == necStrategy) {
            strat = command;
          } else if (address == necEnd) {
            loadedStrat = true;
            break;
          } else if (address == necVar) {
            waitingForVar = true;
          } else if (waitingForVar) {
            uint32_t var = ((uint32_t) address << 16) | ((uint32_t) command << 8) | (inv_command);
            memcpy(&vars[varsC], &var, sizeof(uint32_t));
            varsC++;
            waitingForVar = false;
          }
        }
      }
    } else if (address == necEvent) {
      event = command;
      loadedEvent = true;
    }
    return (loadedStrat || loadedEvent);
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
    _nec_ok = 0;                                  // Reset decoding process
    event = 0;
    _nec_state = 0;
    TCCR1B = 0;                                  // Disable Timer1 module
    address = _nec_code >> 16;
    command = _nec_code >> 8;
    inv_command = _nec_code;
    attachInterrupt(IR_INT, remote_read, CHANGE);
    return true;
  } else return false;
}

uint8_t RMDecode::getStrategy() {
  return strat;
}

uint8_t RMDecode::getEvent() {
  loadedEvent = false;
  uint8_t r = event;
  event = 0;
  return r;
}

ISR(TIMER1_OVF_vect) {                           // Timer1 interrupt service routine (ISR)
  _nec_state = 0;                                 // Reset decoding process
  TCCR1B = 0;                                    // Disable Timer1 module
}
