/*
  RM_Config.h - Configurations file for library RoboMote-Decode
  Created by Paulius Jasėnas, October 19, 2019.
  Released under GNU General Public License v3.0
*/

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

// This library uses Timer2. If it's already taken then...
// Well, tough titties, you'll have to fix it yourself :)
