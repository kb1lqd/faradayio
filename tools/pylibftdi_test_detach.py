from pylibftdi.bitbang import BitBangDevice
for i in range(100):
  bb = BitBangDevice(auto_detach=True)
  bb.close()
  print(i)
