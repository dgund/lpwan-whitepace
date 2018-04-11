#! /usr/bin/env python3
#
# read-serial.py
# Useful for saving the serial output of a LoRaBug radio to a file
#
# MIT License
#
# Copyright (c) 2018 Devin Gund (https://dgund.com)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

import serial

# The serial port of the LoRaBug
SERIAL_PORT = '/dev/tty.usbserial-A6006Q7P'

# The name of the output file to save the serial data
OUTPUT_FILENAME = 'output.txt'

f = open(OUTPUT_FILENAME, 'w+')
ser = serial.Serial(port=SERIAL_PORT, baudrate=115200)

while True:
    try:
        # Continously read serial data and write to the output file
        f.write(ser.readline().decode('utf-8'))
    except:
        # Stop reading when the serial port is disconnected
        break;

f.close()
ser.close()
