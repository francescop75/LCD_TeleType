/**
 * LCD_TeleType
 * A library to use I2C LCD displays as a simple teletype.
 *
 * Copyright (C) 2025 Francesco P. Sileno. All rights reserved.
 */

#ifndef LCDTELETYPE_H
#define LCDTELETYPE_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define VERSION "1.0.2-beta"

/*
 * Implemented backslash sequences:
 */
// Set cursor to home position: 0,0.
#define CMD_HOME 0x00
// Emulate teletype bell by flashing the screen.
#define CMD_BELL '\a'
// Go back 1 character on the current line.
#define CMD_BACKSPACE '\b'
// Clear the screen and set cursor to home position.
#define CMD_CLEAR '\f'
// Newline + carriage return. Scroll the buffer.
#define CMD_NEWLINE '\n'
// Carriage return only, go to the beginning of the current line.
#define CMD_CARRIAGE_RETURN '\r'
// Switch back light off.
#define CMD_BACKLIGHT_OFF 0x0e
// Switch back light on.
#define CMD_BACKLIGHT_ON 0x0f

// Interval between retroillumination on and off.
// #define BELL_DELAY 15
// How many times to repeat the flash.
// #define bell_repetitions 35

class LCDTeleType {

private:
  byte _lcd_address, _bell_delay, _bell_repetitions, _backlight;
  unsigned int _lcd_rows, _lcd_columns;
  LiquidCrystal_I2C *_lcd;

  unsigned int _pos_x, _pos_y;
  char *_buffer;

  unsigned int _offset(unsigned int y, unsigned int x);
  void _scroll();
  void _advance(unsigned int p);
  void _advanceRow(unsigned int r);

public:
  LCDTeleType(
      byte lcd_address,            // LCD display I2C address.
      unsigned int lcd_rows,       // LCD display rows.
      unsigned int lcd_columns,    // LCD display columns.
      byte bell_delay = 15,        // Bell simulation - flash interval.
      byte bell_repetitions = 35); // Bell simulation - flash repetitions.

  void begin();
  void end();
  char *getVersion();

  void clear();
  void redraw();
  void home();
  void prompt();

  void write(char c);
  void write(char *s);
  void write(String s);

  void backspace();
  void newLine();
  void carriageReturn();
  void bell();
  void backlight(byte status);
};

#endif
