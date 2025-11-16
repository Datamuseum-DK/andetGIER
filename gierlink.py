Z0_TIMEOUT = 0.5

import RPi.GPIO as GPIO
import time
import threading

GPIO.setmode(GPIO.BCM) # XXX why not GPIO.BOARD?

# FIXME indsæt de rigtige RPi3 GPIO numre
PIN_Z0      = 24 # PIC18: RB0
PIN_Za      = 23 # PIC18: RB1
PIN_OUTPUTS = [20,16,12,7,8,25,18] # PIC18: RD1-RD7
PIN_OUT1    = 20
PIN_OUT2    = 16
PIN_OUT4    = 12
PIN_OUT8    = 7
PIN_OUT16   = 8
PIN_OUT32   = 25
PIN_OUT64   = 18
PIN_READY   = 27 # PIC18: RE2
PIN_LAMP    = 21 # PIC18: RD0
#PIN_INPUTS  = [6,13,19,26,22,9,5] # PIC18: RC0-RC2,RA0,RA3
#PIN_INPUTS  = [19,26,22,9,5,6,13]
PIN_INPUTS  = [13,6,5,9,22,26,19] 

PIN_IN1     = 13
PIN_IN2     = 6
PIN_IN4     = 5
PIN_IN8     = 9
PIN_IN16    = 22
PIN_IN32    = 26
PIN_IN64    = 19

# setup pins
GPIO.setup(PIN_Z0, GPIO.OUT)
GPIO.setup(PIN_Za, GPIO.IN)

GPIO.setup(PIN_OUT1, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(PIN_OUT2, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(PIN_OUT4, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(PIN_OUT8, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(PIN_OUT16, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(PIN_OUT32, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(PIN_OUT64, GPIO.IN, pull_up_down=GPIO.PUD_UP)

GPIO.setup(PIN_READY, GPIO.OUT)
GPIO.setup(PIN_LAMP,  GPIO.IN, pull_up_down=GPIO.PUD_UP)
#for pin in PIN_INPUTS: GPIO.setup(pin, GPIO,OUT)
GPIO.setup(PIN_IN1, GPIO.OUT)
GPIO.setup(PIN_IN2, GPIO.OUT)
GPIO.setup(PIN_IN4, GPIO.OUT)
GPIO.setup(PIN_IN8, GPIO.OUT)
GPIO.setup(PIN_IN16, GPIO.OUT)
GPIO.setup(PIN_IN32, GPIO.OUT)
GPIO.setup(PIN_IN64, GPIO.OUT)

def set(tal):
	for i, pin in enumerate(PIN_INPUTS): GPIO.output(pin, (tal & (1 << i)) is not 0)


GPIO.output(PIN_Z0, False)
GPIO.output(PIN_READY, False)
#for pin in PIN_INPUTS: GPIO.output(pin, False)
set(0)

def background():
	prev_za = 0
	prev_lamp = 0
	z0_timeout = None

	while True:
		now = time.time()

		lamp = GPIO.input(PIN_LAMP)
		if lamp != prev_lamp: print("LAMP: %d" % lamp) # FIXME er inverteret

		za = GPIO.input(PIN_Za)
		if za and (za != prev_za):
			GPIO.output(PIN_Z0, False)
			z0_timeout = now + Z0_TIMEOUT

		if (z0_timeout is not None) and (now >= z0_timeout):
			GPIO.output(PIN_Z0, True)
			read_value = 0
			for i, pin in enumerate(PIN_OUTPUTS):
				if GPIO.input(pin):
					read_value += (1 << i)
			print("\nREAD", read_value, now)
			z0_timeout = None

		prev_za = za
		prev_lamp = lamp

threading.Thread(target=background).start()


while True:
	tal = input("Send et tal til GIER> ")
	try:
		tal = int(tal)
		if not ((0 <= tal) and (tal < 128)): raise ValueError("ikke mellem 0 og 127")
		#for i, pin in enumerate(PIN_INPUTS):
			#GPIO.output(pin, (tal & (1 << i)) is 0)
		set(tal)
		#time.sleep(0.05)
		GPIO.output(PIN_READY, True)
		time.sleep(0.05) # sleep 50ms
		#for pin in PIN_INPUTS:
			#GPIO.output(pin, False)
		set(0)
		#tal=6
		#for i, pin in enumerate(PIN_INPUTS):
			#GPIO.output(pin, (tal & (1 << i)) is not 0)
		time.sleep(0.05) # sleep 50ms
		GPIO.output(PIN_READY, False)

	except ValueError as e:
		print("ikke lige det bedste tal: %s" % e)

