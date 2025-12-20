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

VERSION = "1.1"
SERIAL = "/dev/ttyLCD"
SERIAL_SPEED = 300
TERMINAL_BELL = b"\a"
TERMINAL_CLEAR = b"\f"
TERMINAL_HOME = b"\x00"
LOOP_DELAY = 10
DEBUG=False

mailboxes = {
    #    "test": {                       # Internal tag - not used.
    #        "server": "my.imap.server", # IMAP server. Implicit SSL connection.
    #        "login": "my.username",     # IMAP login.
    #        "password": "my.password",  # IMAP password.
    #        "folders": [
    #            {
    #                "name": "INBOX",    # IMAP folder name.
    #                "label": "FPS"      # Short label to display on LCD.
    #            },
    #        ],
    #    },
    #    ...
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

    if (DEBUG):
        print("Mailbox: ", M)

    try:

        imap = imaplib.IMAP4_SSL(mailboxes[M]["server"])
        imap.login(
            mailboxes[M]["login"], mailboxes[M]["password"])

        if (DEBUG):
            for i in imap.list()[1]:
                print(i)

        for F in mailboxes[M]["folders"]:
            imap.select(F["name"], readonly=True)
            status, data = imap.search(None, "UNSEEN")
            email_count = len(data[0].split())
            if email_count != 0:
                counters[F["label"]] = email_count

            if (DEBUG):
                print(F)
                print(status, data)

        imap.logout

    except Exception as e:

        pass

    if (DEBUG):
        print("\n---\n")


##
# Main
#

if len(counters):
    now = datetime.now()
    date = now.strftime("%H:%M")
    output = ("\n%s" % date)
    for label, count in counters.items():
        output += (" %s:%d" % (label, count))
    # output += "\n"
    terminal.write(output.encode("utf-8"))
    terminal.write(TERMINAL_BELL)

    if (DEBUG):
        print("LCD: ", output)
