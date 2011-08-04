#!/bin/bash
#scp /home/rainer/workspace/Testsuite-C-Eclipse-Proj/Debug/test.hex 192.168.0.24:/tmp
#ssh 192.168.0.24 "avrdude -p atmega128 -P /dev/parport0 -c stk200 -U flash:w:/tmp/test.hex; exit"

avrdude -p atmega128 -P /dev/parport0 -c stk200 -U flash:w:Debug/test.hex
