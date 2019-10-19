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
/*
void remote_read() {
  unsigned int timer_value;
  if(_nec_state != 0){
    timer_value = TCNT1;                         // Store Timer1 value
    TCNT1 = 0;                                   // Reset Timer1
  }
  switch(_nec_state){
   case 0 :                                      // Start receiving IR data (we're at the beginning of 9ms pulse)
    TCNT1  = 0;                                  // Reset Timer1
    TCCR1B = 2;                                  // Enable Timer1 module with 1/8 prescaler ( 2 ticks every 1 us)
    _nec_state = 1;                               // Next state: end of 9ms pulse (start of 4.5ms space)
    _i = 0;
    return;
   case 1 :                                      // End of 9ms pulse
    if((timer_value > 19000) || (timer_value < 17000)){         // Invalid interval ==> stop decoding and reset
      _nec_state = 0;                             // Reset decoding process
      TCCR1B = 0;                                // Disable Timer1 module
    }
    else
      _nec_state = 2;                             // Next state: end of 4.5ms space (start of 562µs pulse)
    return;
   case 2 :                                      // End of 4.5ms space
    if((timer_value > 10000) || (timer_value < 8000)){
      _nec_state = 0;                             // Reset decoding process
      TCCR1B = 0;                                // Disable Timer1 module
    }
    else
      _nec_state = 3;                             // Next state: end of 562µs pulse (start of 562µs or 1687µs space)
    return;
   case 3 :                                      // End of 562µs pulse
    if((timer_value > 1400) || (timer_value < 800)){           // Invalid interval ==> stop decoding and reset
      TCCR1B = 0;                                // Disable Timer1 module
      _nec_state = 0;                             // Reset decoding process
    }
    else
      _nec_state = 4;                             // Next state: end of 562µs or 1687µs space
    return;
   case 4 :                                      // End of 562µs or 1687µs space
    if((timer_value > 3600) || (timer_value < 800)){           // Time interval invalid ==> stop decoding
      TCCR1B = 0;                                // Disable Timer1 module
      _nec_state = 0;                             // Reset decoding process
      return;
    }
    if( timer_value > 2000)                      // If space width > 1ms (short space)
      bitSet(_nec_code, (31 - _i));                // Write 1 to bit (31 - i)
    else                                         // If space width < 1ms (long space)
      bitClear(_nec_code, (31 - _i));              // Write 0 to bit (31 - i)
    _i++;
    if(_i > 31){                                  // If all bits are received
      _nec_ok = 1;                                // Decoding process OK
      detachInterrupt(IR_INT);                        // Disable external interrupt (INT0)
      return;
    }
    _nec_state = 3;                               // Next state: end of 562µs pulse (start of 562µs or 1687µs space)
  }
}
*/

RMDecode::RMDecode() {
}

void RMDecode::begin() {
  timeoutTimer = millis();
  varsC = 0;
  strat = 0;
  loaded = false;

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
      loaded = false;
      bool waitingForVar = false;

      while (timeoutTimer > millis() && m < msgs) {
        if (getMessage()) {
          m++;
          if (address == necStrategy) {
            strat = command;
          } else if (address == necEnd) {
            loaded = true;
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
    }
    return loaded;
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

ISR(TIMER1_OVF_vect) {                           // Timer1 interrupt service routine (ISR)
  _nec_state = 0;                                 // Reset decoding process
  TCCR1B = 0;                                    // Disable Timer1 module
}
