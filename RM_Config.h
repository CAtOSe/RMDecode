/*
  RM_Config.h - Configurations file for library RoboMote-Decode
  Created by Paulius Jasėnas, October 19, 2019.
  Released under GNU General Public License v3.0
*/

// Select interrupt pin for your IR receiver:
// This should be written in your MCUs datasheet as INT[PIN NUM]. ex: INT0
// ATmega328:
//   0 - D2
//   1 - D3
//
// ATmega2560:
//   0 - D2
//   1 - D3
//   2 - D21
//   3 - D20
//   4 - D19
//   5 - D18
//
// ATmega32U4:
//   0 - D3
//   1 - D2
//   2 - D0
//   3 - D1
//   4 - D7
#define IR_INT 0



// How many variables will the library load?
// NOTE: Increasing this, will increase your RAM usage
// HOW MUCH THIS USES: MAX_VARIABLES * 32 bits
#define MAX_VARIABLES 10

// How long unitl an unfinished message is abandoned
// NOTE: If this is too short, message will be choped off
// Time in milliseconds
#define MSG_TIMEOUT 3000

// Addresses of RoboMote
#define NEC_ADDR 0xABC
#define NEC_START 0x1
#define NEC_STRATEGY 0x2
#define NEC_VAR 0x3
#define NEC_EVENT 0x4
#define NEC_END 0x6

// This library uses Timer1. If it's taken already...
// Well, tough titties, you'll have to fix it yourself
