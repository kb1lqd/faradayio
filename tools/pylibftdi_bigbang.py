from pylibftdi import BitBangDevice
from pylibftdi import driver

# Append new USB PID number to the USB_PID_LIST
driver.USB_PID_LIST.append(0x6015)

with BitBangDevice() as bb:
    bb.direction = 0x00
    bb.port &= 0xFF
    #bb.port |= 0xFF

print "Completed."
