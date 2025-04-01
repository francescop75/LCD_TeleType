##
# To use this Makefile, you must have arduino-cli and arduino-lint.
# https://arduino.github.io/arduino-cli/latest/
# https://arduino.github.io/arduino-lint/latest/
##

BOARD = arduino:avr:diecimila
PORT = /dev/ttyUSB0

all: example example_upload

check:
	arduino-lint --library-manager update

example:
	arduino-cli compile --fqbn $(BOARD) examples/LCD_TeleType_Serial

example_upload:
	arduino-cli upload --port $(PORT) --verbose --fqbn $(BOARD) examples/LCD_TeleType_Serial
