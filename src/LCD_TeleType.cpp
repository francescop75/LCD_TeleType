/*
 * LCD_TeleType
 * A library to use I2C LCD displays as a simple teletype.
 *
 * Copyright (C) 2025 Francesco P. Sileno. All rights reserved.
 */

#include "LCD_TeleType.h"

/*
 * Contructor.
 */
LCDTeleType::LCDTeleType(unsigned char lcd_address, unsigned int lcd_rows,
                         unsigned int lcd_columns, unsigned char bell_delay,
                         unsigned char bell_repetitions) {

  // I2C Screen parameters.
  _lcd_address = (unsigned char)lcd_address;
  _lcd_rows = (unsigned int)lcd_rows;
  _lcd_columns = (unsigned int)lcd_columns;
  _bell_delay = (unsigned char)bell_delay;
  _bell_repetitions = (unsigned char)bell_repetitions;
  _backlight = 1;

  _buffer = (char *)malloc(_lcd_rows * _lcd_columns); // Screen buffer.
  _pos_x = _pos_y = 0;                                // Screen cursor position.

  _lcd = new LiquidCrystal_I2C(_lcd_address, _lcd_columns, _lcd_rows);
}

/*
 * LCD initialization.
 */
void LCDTeleType::begin() {
  _lcd->init();
  _lcd->clear();
  _lcd->blink();
  _lcd->backlight();

  _lcd->setCursor(0, 0);
}

void LCDTeleType::end() { free(_buffer); }

/*
 * Return library version.
 */
char *LCDTeleType::getVersion() { return (char *)VERSION; }

/*
 * Clear screen, reset cursor position.
 */
void LCDTeleType::clear() {

  _pos_x = _pos_y = 0;
  memset(_buffer, ' ', (_lcd_rows * _lcd_columns));
  redraw();
}

/*
 * Redraw screen buffer, no screen clear.
 */
void LCDTeleType::redraw() {

  for (unsigned int y = 0; y < _lcd_rows; y++) {

    _lcd->setCursor(0, y);
    for (unsigned int x = 0; x < _lcd_columns; x++) {

      _lcd->print(_buffer[_offset(y, x)]);
    }
  }

  _lcd->setCursor(_pos_x, _pos_y);
}

/*
 * Cursor in home position, no screen clear.
 */
void LCDTeleType::home() {
  _pos_x = _pos_y = 0;
  _lcd->setCursor(_pos_x, _pos_y);
}

/*
 * Output library version and a READY prompt.
 */
void LCDTeleType::prompt() {
  write((char *)"v");
  write(getVersion());
  write((char *)" READY\n");
  bell();
}

/*
 * Write a character to the LCD and in the buffer.
 * ref.: https://www.ascii-code.com/
 */
void LCDTeleType::write(char c) {

  switch (c) {

  case CMD_HOME:
    home();
    break;

  case CMD_BELL:
    bell();
    break;

  case CMD_BACKSPACE:
    backspace();
    break;

  case CMD_CLEAR:
    clear();
    break;

  case CMD_NEWLINE:
    newLine();
    break;

  case CMD_CARRIAGE_RETURN:
    carriageReturn();
    break;

  case CMD_BACKLIGHT_OFF:
    backlight(0);
    break;

  case CMD_BACKLIGHT_ON:
    backlight(1);
    break;

  default:
    _buffer[_offset(_pos_y, _pos_x)] = c;
    _lcd->print(c);
    _advance(1);
  }
}

/*
 * Write a character to the LCD and in the buffer.
 */
void LCDTeleType::write(char *s) {

  /*
   * This way there could be buffer overflows. Should implement boundary
   * checks.
   */
  // memcpy((char*)(_buffer + _offset(_pos_y, _pos_x)), (char*)s,
  //  strlen(s)); _lcd->print(s); _advance(strlen(s));

  for (unsigned int x = 0; x < strlen(s); x++) {
    write((char)s[x]);
  }
}

/*
 * Write a string to the LCD and in the buffer.
 */
void LCDTeleType::write(String s) {

  for (unsigned int x = 0; x < s.length(); x++) {
    write((char)s[x]);
  }
}

/*
 * Back one character. Do not change line.
 */
void LCDTeleType::backspace() {

  _pos_x = (--_pos_x < 0) ? 0 : _pos_x;
  _lcd->setCursor(_pos_x, _pos_y);
}

/*
 * Advance to the beginning of the next line.
 */
void LCDTeleType::newLine() { _advanceRow(1); }

/*
 * Back to the beginning of the line.
 */
void LCDTeleType::carriageReturn() {

  _pos_x = 0;
  _lcd->setCursor(_pos_x, _pos_y);
}

/*
 * Simulate teletype bell with screen flashing.
 */
void LCDTeleType::bell() {

  for (unsigned int x = 0; x < _bell_repetitions; x++) {

    _lcd->noBacklight();
    delay(_bell_delay);
    _lcd->backlight();
    delay(_bell_delay);

    if (_backlight) {
      _lcd->backlight();
    } else {
      _lcd->noBacklight();
    }
  }
}

void LCDTeleType::backlight(unsigned char status) {

  if (status) {

    _backlight = 1;
    _lcd->backlight();

  } else {

    _backlight = 0;
    _lcd->noBacklight();
  }
}

/*
 * PRIVATE FUNCTIONS.
 */

/*
 * Buffer offset by cursor position.
 */
unsigned int LCDTeleType::_offset(unsigned int y, unsigned int x) {

  return ((y * _lcd_columns) + x);
}

/*
 * Scroll upward one line of text.
 */
void LCDTeleType::_scroll() {

  for (unsigned int x = 0; x < (_lcd_rows - 1); x++) {
    memcpy((char *)(_buffer + _offset(x, 0)),
           (char *)(_buffer + _offset(x + 1, 0)), _lcd_columns);
  }
  memset((char *)(_buffer + _offset((_lcd_rows - 1), 0)), ' ', _lcd_columns);

  redraw();
}

/*
 * Increase cursor position and scroll if necessary.
 */
void LCDTeleType::_advance(unsigned int p) {

  if ((_pos_x += p) >= _lcd_columns) {

    _pos_x = 0;

    if ((++_pos_y) == _lcd_rows) {

      _pos_y--;
      _scroll();
    }

    _lcd->setCursor(_pos_x, _pos_y);
  }
}

// Advance cursor to the beginning of the next "r" rows. Scroll if necessary.
void LCDTeleType::_advanceRow(unsigned int r) {

  _pos_x = 0;
  if ((_pos_y += r) >= _lcd_rows) {

    _pos_y--;
    _scroll();
  }

  _lcd->setCursor(_pos_x, _pos_y);
}
