##
# To use this Makefile, you must have arduino-cli and arduino-lint.
# https://arduino.github.io/arduino-cli/latest/
# https://arduino.github.io/arduino-lint/latest/
#

# Your board type, see "arduino-cli board listall" and try identification with "arduino-cli board list".
BOARD = arduino:avr:diecimila

# Your board USB TTY port. If using UDEV rule (see udev/99-LCD_Terminal.rules), you can use your custom device.
PORT = /dev/ttyUSB0

all: example_upload

check:
	arduino-lint --library-manager update

example:
	arduino-cli compile --fqbn $(BOARD) examples/LCD_TeleType_Serial

example_upload: example
	arduino-cli upload --port $(PORT) --verbose --fqbn $(BOARD) examples/LCD_TeleType_Serial

