/*
  RMDecode.h - Library for decoding RoboMote signals.
  Created by Paulius Jasėnas, October 19, 2019.
  Released under GNU General Public License v3.0
*/
#ifndef RMDecode_h
#define RMDecode_h

#include "Arduino.h"
#include "RM_Config.h"

class RMDecode
{
  public:
    RMDecode();
    void begin();
    void stop();
    bool decode();
    uint8_t getStrategy();
    uint8_t getVarCount();
    bool getBool(uint8_t index);
    long getInt(uint8_t index);
    float getFloat(uint8_t index);
    uint32_t getRaw(uint8_t index);
  private:
    bool getMessage();

    uint16_t address;
    uint8_t command;
    uint8_t inv_command;

    uint32_t timeoutTimer;
    uint8_t msgs;

    bool loaded;
    uint8_t strat;
    long vars[MAX_VARIABLES];
    uint8_t varsC;
};

static const uint16_t necStart = (NEC_ADDR << 4) | NEC_START;
static const uint16_t necStrategy = (NEC_ADDR << 4) | NEC_STRATEGY;
static const uint16_t necVar = (NEC_ADDR << 4) | NEC_VAR;
static const uint16_t necEnd = (NEC_ADDR << 4) | NEC_END;

#endif
