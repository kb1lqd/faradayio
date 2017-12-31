from pyftdi.ftdi import Ftdi
from time import sleep

ftdi1 = Ftdi()
ftdi1.open(vendor=0x403, product=0x6015, interface=1)

vps=[(0x0403,0x6015)]

for x in range(5):
    print("If#1: ", hex(ftdi1.poll_modem_status()))
    print("Read Pins:", bin(ftdi1.read_pins()))
    print("Devices:", bin(ftdi1.find_all(vps)))
    sleep(0.500)

ftdi1.close()
