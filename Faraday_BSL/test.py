import travisgoodspeeddirtybsl

test_bsl = travisgoodspeeddirtybsl.BSL('/dev/ttyUSB0')

print test_bsl

# Try to enter BSL mode
test_bsl.enter_bsl()

print "BSl mode entered."

# Try to exit BSL mode
test_bsl.reset()

print "BSl mode exited."
