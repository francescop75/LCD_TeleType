#!/bin/bash

##
# Initialize the LCD Teletype device port for use from shell.
#

LCD_DEVICE=/dev/ttyLCD
LCD_SPEED=300

stty -F ${LCD_DEVICE} clocal -parenb -parodd -cmspar cs8 -hupcl -cstopb cread -crtscts -ixon -ixoff ${LCD_SPEED}
sleep 1
echo -en "\f" > ${LCD_DEVICE}

