from pylibftdi import Device

with Device(mode='t') as dev:
    dev.baudrate = 115200
    dev.write('Hello World')
