# RMDecoder
RoboMote Decoder. Decodes signals used by RoboMote. Basically Infrared signals with NEC protocol.

## How to use
Connect your IR receiver to an interrupt pin.  
If you don't know which pin is an interrupt, please refer to your MCUs datasheet.
Change your interrupt pin in `RM_Config.h`
```c
#define IR_INT 0
```

Change your address used for receiving in `RM_Config.h` It must be 12 bits (3 HEX).
```c
#define NEC_ADDR 0xABC
```

Upload example `SimpleRcv` and enjoy.

## Other
If you find any bugs, please open an issue here and provide as much info as possible. Thanks!
Licensed under GNU General Public License v3.0.
