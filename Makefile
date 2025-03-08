
BOARD = arduino:avr:diecimila
PORT = /dev/ttyUSB0

all: example example_upload

example:
	arduino-cli compile --fqbn $(BOARD) examples/LCD_TeleType_Serial

example_upload:
	arduino-cli upload --port $(PORT) --verbose --fqbn $(BOARD) examples/LCD_TeleType_Serial
