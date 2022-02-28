# cctrl
c++20 for the Arduino
This is a small experimental project experimenting with std library on avr.
Tested (only) with gcc 11.2

Implemented was a small logger, to be found at libraries/utl/include/utl/logger.h
Usage of the logger can be found at <b>mockup/src/mockup.cpp</b> or <b>application/src/cctrl.cpp</b>

This git repo uses submodules, before compiling, update them!


To create the build directory use:
```
./generate
```
This will create the directories <b>build/avr</b> for compiling for AVR, and <b>build/x86</b> for building for x86 / native. Find the mockup application at <b>build/x86/mockup/mockup</b>
