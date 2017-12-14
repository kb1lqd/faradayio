"""
A quick dirty pylibftdi implementation of adding an FT230X vendor product
ID to the pylibftdi valid ID list. Run as sudo or with proper USB permissions.
The program will simply list out connected devices as long as thier product ID's
are in the valid list. This is a copy of the list_devices pylibftdi example
except exampled is actively adding new ID's.
"""

import pylibftdi

def get_ftdi_device_list():
    """
    return a list of lines, each a colon-separated
    vendor:product:serial summary of detected devices
    """
    dev_list = []
    for device in pylibftdi.Driver().list_devices():
        # list_devices returns bytes rather than strings
        dev_info = map(lambda x: x.decode('latin1'), device)
        # device must always be this triple
        vendor, product, serial = dev_info
        dev_list.append("%s:%s:%s" % (vendor, product, serial))
    return dev_list


def main():
    # Print as imported USB_PID_LIST
    print "Imported USB_PID_LIST:", map(hex,pylibftdi.USB_PID_LIST)

    # Append new USB PID number to the USB_PID_LIST
    pylibftdi.USB_PID_LIST.append(0x6015)

    # Print updated USB_PID_LIST
    print "Updated USB_PID_LIST:", map(hex,pylibftdi.USB_PID_LIST)
    print pylibftdi.Driver().list_devices()

    for device in get_ftdi_device_list():
        print "Devices:", (device)


if __name__ == "__main__":
    main()
