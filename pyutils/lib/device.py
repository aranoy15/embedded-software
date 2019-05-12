#!/usr/bin/env python
# coding: utf-8

import serial
import serial.tools.list_ports
import lib.config as config
import time

class Device(object):

    ENCODING = 'ascii'
    DECODE_ERRORS = "backslashreplace"

    def __init__(self, port):
        self.device = None
        self.port = port

    def __open(self, baud=config.DEFAULT_BAUD):
        self.device = serial.Serial()
        self.device.port = self.port
        self.device.baudrate = baud
        self.device.bytesize = 8
        self.device.parity = 'N'
        self.device.stopbits = 1
        self.device.xonxoff = False
        self.device.rtscts = False
        self.device.dsrdtr = False
        self.device.timeout = 1
        self.device.rts = False
        self.device.dtr = False
        self.device.open()

    def close(self):
        if self.device is not None:
            self.device.close()
            self.device = None
    
    def read_line(self, need_original=False, timeout=0.1):
        if self.wait_for_data(timeout):
            line = self.device.readline()

            if need_original:
                return line
            
            return line.decode(encoding=Device.ENCODING, errors=Device.DECODE_ERRORS)

    def write(self, data):
        if isinstance(data, str):
            data = bytes(data, Device.ENCODING)
        self.device.write(data)

    def wait_for_data(self, timeout):
        i = 0
        start = time.time()
        while time.time() - start < timeout:
            i = self.device.in_waiting
            if i != 0:
                break
                
        return i

    def open(self, baud=config.DEFAULT_BAUD):

        self.__open(baud)
