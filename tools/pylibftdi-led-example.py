"""
Flash an LED connected via a FTDI UM232R/245R module using pylibftdi

Optionally supply a flash rate (in Hz, default 1) as an argument

Copyright (c) 2010-2014 Ben Bass <benbass@codedstructure.net>
All rights reserved.
"""

import time
import sys
from pylibftdi import BitBangDevice
from pylibftdi import driver

# Append new USB PID number to the USB_PID_LIST
print driver.USB_PID_LIST
driver.USB_PID_LIST.append(0x6015)
print driver.USB_PID_LIST

def flash_forever(rate):
    """toggle bit zero at rate Hz"""
    # put an LED with 1Kohm or similar series resistor
    # on D0 pin
    # Default is all outputs?
    with BitBangDevice() as bb:
        while True:
            #time.sleep(1.0 / (2 * rate))
            time.sleep(3.0)
            bb.port ^= 0xff
            print "Toggling."


def main():
    if len(sys.argv) > 1:
        rate = float(sys.argv[1])
        flash_forever(rate)
    else:
        flash_forever(1)


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
