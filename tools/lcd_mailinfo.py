#!/usr/bin/env python3

##
# Print a compact INBOX mail summary.
# To be used in cron.
#

import imaplib
import psutil
import serial
import sys
from datetime import datetime
from time import sleep

VERSION = "1.0"
SERIAL = "/dev/ttyLCD"
SERIAL_SPEED = 300
TERMINAL_BELL = b"\a"
TERMINAL_CLEAR = b"\f"
TERMINAL_HOME = b"\x00"
LOOP_DELAY = 10

mailboxes = {
#    "test": {                       # Internal tag - not used.
#        "label": "TST",             # Printed short label for mailbox.
#        "server": "my.imap.server", # IMAP server. Implicit SSL connection.
#        "login": "my.username",     # IMAP login.
#        "password": "my.password",  # IMAP password.
#    },
#   ...
}

##
# Serial communication setup.

try:
    terminal = serial.Serial(SERIAL, SERIAL_SPEED, rtscts=0)
    sleep(1)
except Exception as e:
    sys.exit()

##
# Mailboxes loop.
#

counters = {}
for M in mailboxes:

    imap = imaplib.IMAP4_SSL(mailboxes[M]["server"])
    imap.login(
        mailboxes[M]["login"], mailboxes[M]["password"])
    imap.select("INBOX", readonly=True)

    try:
        status, data = imap.search(None, "UNSEEN")
        email_count = len(data[0].split())

        if email_count != 0:

            counters[mailboxes[M]["label"]] = email_count

    except Exception as e:

        pass

    imap.logout

##
# Main
#

if len(counters):
    now = datetime.now()
    date = now.strftime("%H:%M")
    output = ("%s" % date)
    for label, count in counters.items():
        output += (" %s:%d" % (label, count))
    output += "\n"
    terminal.write(output.encode("utf-8"))
    terminal.write(TERMINAL_BELL)

    # Debug output.
    # print(output)
