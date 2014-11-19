#!/usr/bin/python
#
# Creates a C code lookup table for doing gamma correction on LEDs on a microcontroller
# based on: http://hydraraptor.blogspot.com/2007/10/measuring-temperature-easy-way.html
"""Gamma Correction Lookup Table Generator



Usage: python createGammaLookupTable.py [options]

Options:
  -h, --help			show this help
  --gamma=... 			gamma correction value (eg: 1.0 - 3.0)
  --max-pwm=... 		the PWM value to use.  (default: 255)
  --count=... 			the number of values to generate (default: 64)
"""

from math import *
import sys
import getopt

class GammaCorrector:
	"Class to do the gamma maths"
	def __init__(self, gamma, max_pwm, max_counts, offset):
		self.gamma = gamma
		self.max_pwm = max_pwm
		self.max_counts = max_counts
		self.offset = offset
		
	def correct(self, i):
		"Convert count into appropriate value"
		return max_counts * ((max_pwm - offset) ^ gamma) + offset;

	def get_values(self):
		for i

def main(argv):

	gamma = 2.5;
	max_pwm = 255;
	max_counts = 64;
	
	try:
		opts, args = getopt.getopt(argv, "h", ["help", "gamma=", "max-pwm=", "count="])
	except getopt.GetoptError:
		usage()
		sys.exit(2)
        
	for opt, arg in opts:
		if opt in ("-h", "--help"):
			usage()
			sys.exit()
		elif opt == "--gamma":
		 	gamma = int(arg)
		elif opt == "--max-pwm":
			max_pwm = int(arg)
		elif opt == "--count":
		 	count = int(arg)
			
	gc = GammaCorrector(gamma, max_pwm, max_counts, 0);
	
	gamma_values = gc.getValues();
	
	first = 1

	print "// Gamma lookup table for Arduino PWM Control (http://make.rrrf.org/ts)"
#	print "// Made with createTemperatureLookup.py (http://svn.reprap.org/trunk/reprap/firmware/Arduino/utilities/createTemperatureLookup.py)"
#	print "// ./createTemperatureLookup.py --r0=%s --t0=%s --r1=%s --r2=%s --beta=%s --max-adc=%s" % (r0, t0, r1, r2, beta, max_adc)
#	print "// r0: %s" % (r0)
#	print "// t0: %s" % (t0)
#	print "// r1: %s" % (r1)
#	print "// r2: %s" % (r2)
#	print "// beta: %s" % (beta)
#	print "// max adc: %s" % (max_adc)
	print "#define GAMMA_VALUE_COUNT %s" % (len(adcs))
	print "short temptable[GAMMA_VALUE_COUNT] = {"

	counter = 0
	for gamma in gamma_values:
		counter = counter +1
		if counter == len(gamma_values):
			print "   %s" % (gamma)
		else:
			print "   %s," % (gamma)
	print "};"
	
def usage():
    print __doc__

if __name__ == "__main__":
	main(sys.argv[1:])