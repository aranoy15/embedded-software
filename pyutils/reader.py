#!/usr/bin/env python
# coding: utf-8

import getopt
import serial
import sys
import threading
import lib.device
import datetime
import shutil
from lib.commands import Commands
from lib.device import Device


def main(argv):
    try:
        optlist, args = getopt.getopt(argv[1:], 'b:hc:l')
    except getopt.GetoptError as err:
        print("Error: " + str(err))
        help_info()
        sys.exit(1)

    opts = dict(optlist)
    if '-h' in opts:
        help_info()
        sys.exit(0)

    port = args[0]
    device = Device(port)

    if '-b' in opts:
        device.open(int(opts.get('-b')))
    else:
        device.open()

    isatty = sys.stdout.isatty()
    iscmd = isatty

    if iscmd:
        cli = Commands(device)
        cmdlinethevt = threading.Event()
        cmdlinethevt.clear()
        cmdlineth = threading.Thread(target=cmd_line, args=(cli, cmdlinethevt),
                                     daemon=True)
        cmdlineth.start()
        prevprompt = ""

    try:
        while True:
            line = device.read_line()

            if line:
                line = line.strip('\r\n')

                if iscmd: 
                    clearline()

                print('[{0}] {1}'.format(datetime.datetime.now().strftime('%H:%M:%S.%f'), line))
                sys.stdout.flush()

            if iscmd:
                prompt = " > {0}".format(
                    cli.command_buf()
                )

                if line or prompt != prevprompt:
                    updateprompt(prompt, cli.cursor_pos)
                    prevprompt = prompt

                if cli.interrupt:
                    raise KeyboardInterrupt

    except KeyboardInterrupt:
        pass
    except serial.serialutil.SerialException as ex:
        sys.stderr.write("\n" + str(ex) + "\n")
    finally:
        device.close()

        if isatty:
            cmdlinethevt.set()
            cmdlineth.join()
            cli.cleanup()

def cmd_line(cli, evt):
    """
    Read keyboard from another thread
    """
    while not evt.is_set():
        cli.process_command()

def updateprompt(prompt, cursor_pos):
    """
    Show prompt and move cursor
    """
    clearline()
    sys.stdout.write(prompt)
    for _ in range(cursor_pos):
        sys.stdout.write('\033[D')
    sys.stdout.flush()

def clearline():
    """
    Clear command line to write log line
    """
    sys.stdout.write("\r")
    sys.stdout.write(" " * (shutil.get_terminal_size().columns - 1))
    sys.stdout.write("\r")

def help_info():
    """
    Print help info message
    """

    print(
        'reader [-b <baudrate>] device',
        "\n\nOptions:\n"
        "\t-h               -show this message\n"
        "\t-b <baudrate>    -open the device with baudrate (Default: 115200)\n"
    ) 

if __name__ == '__main__':
    main(sys.argv)
