#!/bin/bash
/usr/bin/avrdude -q -V -p atmega328p -C /etc/avrdude.conf -D -c arduino -b 115200 -P /dev/ttyACM0 \
			-U flash:w:build/cctrl.hex:i

