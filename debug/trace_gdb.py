#!/usr/bin/env python
# coding: utf-8
#      test.gdb.py
"""
"""

import sys


def main(argv):
    # gdb.execute("set logging on")
    gdb.execute("target remote localhost:3333")
    gdb.execute("break main")
    #gdb.execute("break uartbuffer.cpp:13")
    #gdb.execute("break app.cpp:initAll")
    gdb.execute("monitor reset halt")
    gdb.execute("continue")
    while True:
        gdb.execute("step")
        # gdb.execute("locals")
        # gdb.execute("continue")


if __name__ == "__main__":
    main(sys.argv)
