#!/usr/bin/env python
# coding: utf-8

import sys
import termios
import tty
import re
import select
import string

class Commands(object):
    history = None
    history_index = 0
    interrupt = False
    stdin_settings = None
    cursor_pos = 0
    saved_command = None

    def __init__(self, device):
        self.device = device
        self.history = []
        self.current_command = ''

        self.special_keys = {
            3: self.do_exit,  # ctrl+c
            4: self.do_exit,  # ctrl+d
            23: self.do_backword,  # ctrl+w
            127: self.do_backspace,  # backspace
            8: self.do_backspace,  # backspace in windows
            '\033[3~': self.do_delete,  # delete
            13: self.do_enter,  # enter
            10: self.do_down,  # ctrl+j
            11: self.do_up,  # ctrl+k
            '\033[A': self.do_up,  # up
            '\033[B': self.do_down,  # down
            '\033[C': self.do_right,  # right
            '\033[D': self.do_left,  # left
            1: self.do_home,  # ctrl+a
            5: self.do_end,  # ctrl+e
            '\033[1~': self.do_home,  # home
            '\033[4~': self.do_end,  # end
            '\t': self.do_complete,  # Tab
        }

    def command_buf(self):
        """
        Return current command line buffer
        """
        return self.current_command

    def process_command(self):
        self.current_command = self.__process_command(self.current_command)
        #print('command_buf' + self.current_command)
    
    def __process_command(self, command):
        """
        Read and process character from the keyboard
        """
        char = self.getch()
        if char is not None:
            #print("+" + str([c for c in char]) + "+")
            #print("+" + str(char) + "+")
            if str(char) in self.special_keys:
                return self.special_keys[str(char)](command)

            ochar = ord(char[0])
            if ochar in self.special_keys:
                return self.special_keys[ochar](command)

            # print("+" + str(ochar) + "+")
            if char in string.printable:
                pos = len(command) - self.cursor_pos
                return command[0:pos] + char + command[pos:]

        return command

    def do_exit(self, command):
        """
        Exit
        """
        self.interrupt = True
        return command

    def do_backword(self, command):
        """
        Remove last word
        """
        rpos = len(command) - self.cursor_pos
        try:
            lpos = command[0:rpos].rstrip().rindex(" ", 0, rpos) + 1
        except ValueError:
            lpos = 0

        newcmd = command[0:lpos] + command[rpos:]
        return newcmd

    def do_backspace(self, command):
        """
        Remove character before cursor from the buffer
        """
        pos = len(command) - self.cursor_pos
        if pos <= 0:
            return command
        newcmd = command[0:pos - 1] + command[pos:]
        return newcmd

    def do_delete(self, command):
        """
        Remove character after cursor from the buffer
        """
        if self.cursor_pos == 0:
            return command

        pos = len(command) - self.cursor_pos
        newcmd = command[0:pos] + command[pos + 1:]
        self.cursor_pos -= 1
        return newcmd

    def do_enter(self, command):
        """
        Execute current command
        """
        if command:
            if self.history == [] or command != self.history[-1]:
                self.history.append(command)
            self.history_index = len(self.history)

            if command == "!reset" or command == "!dtr":
                self.device.reset()
            elif command == "!q" or command == "!quit" or command == "!exit":
                self.do_exit(command)
            else:
                self.device.write(command + "\n")
        self.cursor_pos = 0
        return ""

    def do_down(self, command):
        """
        Next command from the history
        """
        self.cursor_pos = 0
        self.history_index += 1
        if self.history_index >= len(self.history):
            self.history_index = len(self.history)
            if self.saved_command is not None:
                cmd = self.saved_command
                self.saved_command = None
            return cmd

        return self.history[self.history_index]

    def do_up(self, command):
        """
        Previous command from the history
        """
        self.cursor_pos = 0

        if self.history == []:
            return command

        if self.history_index == len(self.history):
            self.saved_command = command

        self.history_index -= 1
        if self.history_index <= 0:
            self.history_index = 0
        return self.history[self.history_index]

    def do_left(self, command):
        """
        Catch left arrow key
        """
        if self.cursor_pos < len(command):
            self.cursor_pos += 1
        return command

    def do_right(self, command):
        """
        Catch right arrow key
        """
        if self.cursor_pos > 0:
            self.cursor_pos -= 1
        return command

    def do_end(self, command):
        """
        Move cursor to the end
        """
        self.cursor_pos = 0
        return command

    def do_home(self, command):
        """
        Move cursor to the begin
        """
        self.cursor_pos = len(command)
        return command

    def do_complete(self, command):
        """
        Try to complete the command
        """
        for _pcmd in self.history:
            pcmd = _pcmd.split(" ")[0]
            if pcmd.startswith(command):
                self.cursor_pos = 0
                return pcmd + " "
        return command

    def getch(self):
        """
        Get keyboard char
        """

        filed = sys.stdin.fileno()
        self.stdin_settings = termios.tcgetattr(filed)
        tty.setraw(sys.stdin.fileno())
        try:
            i, _, _ = select.select([sys.stdin], [], [], 0.01)
            if i:
                char = sys.stdin.read(1)
                if char == "\033":
                    char += sys.stdin.read(2)
                    if re.match(r"\033\[\d", char):
                        char += sys.stdin.read(1)
                return char
        finally:
            termios.tcsetattr(filed, termios.TCSADRAIN, self.stdin_settings)

        return None

    def cleanup(self):
        """
        Call before exit
        """
        if self.stdin_settings is not None:
            termios.tcsetattr(sys.stdin.fileno(), termios.TCSADRAIN,
                              self.stdin_settings)