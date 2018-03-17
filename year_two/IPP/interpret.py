"""
Interpret of IPPcode18
Author: Patrik Holop
"""
from xml.etree import ElementTree
import sys


WRONG_ARG = 10
NAME_ERROR = 1
CODE_OK = 0


class Interpret:
    """ Basic interpret of IPPcode18 """
    def __init__(self):
        self.frames = {'GF': [{}], 'LF': [{}], 'TF': [{}]}
        self.data_stack = []
        self.instructions = []
        self.labels = {}
        self.pc = 0
        self.callbacks = []
        self.language_map = {
            "MOVE": self.i_move,
            "CREATEFRAME": self.i_createframe,
            "PUSHFRAME": self.i_pushframe,
            "POPFRAME": self.i_popframe,
            "DEFVAR": self.i_defvar,
            "CALL": self.i_call,
            "PUSHS": self.i_pushs,
            "RETURN": self.i_return,
            "POPS": self.i_pops,
            "ADD": self.i_add,
            "SUB": self.i_sub,
            "MUL": self.i_mul,
            "IDIV": self.i_idiv,
            "WRITE": self.i_write,
            "READ": self.i_read,
            "LT": self.i_lt,
            "GT": self.i_gt,
            "EQ": self.i_eq,
            "AND": self.i_and,
            "OR": self.i_or,
            "NOT": self.i_not,
            "INT2CHAR": self.i_int2char,
            "STRI2INT": self.i_stri2int,
            "CONCAT": self.i_concat,
            "STRLEN": self.i_strlen,
            "GETCHAR": self.i_getchar,
            "SETCHAR": self.i_setchar,
            "TYPE": self.i_type,
            "LABEL": self.nop,
            "JUMP": self.i_jump,
            "JUMPIFEQ": self.i_jumpifeq,
            "JUMPIFNEQ": self.i_jumpifneq,
            "DPRINT": self.i_dprint,
            "BREAK": self.i_break,
        }

    def load_labels(self):
        self.pc = 0
        for instruction in self.instructions:
            opcode = instruction.attrib['opcode'].upper()
            if opcode == "LABEL":
                self.i_label(instruction)
            self.pc += 1
        self.pc = 0

    def add_instruction(self, instruction):
        self.instructions.append(instruction)

    def exec_program(self):
        while True:
            instruction = self.instructions[self.pc]
            self.exec_instruction(instruction)
            if self.pc >= len(self.instructions):
                #self.print_stacks()
                sys.exit(CODE_OK)

    def exec_instruction(self, instruction):
        #print('Executing: ', instruction.attrib)
        func = self.language_map.get(instruction.attrib['opcode'].upper(), "ERROR")
        func(instruction)
        #self.print_stacks()
        self.pc += 1

    def print_stacks(self):
        print('GF:', self.frames['GF'])
        print('LF:', self.frames['LF'])
        print('TF:', self.frames['TF'])

    def i_createframe(self, instruction):
        self.frames['TF'][-1] = {}

    def i_pushframe(self, instruction):
        self.frames['LF'].append(self.frames['TF'][-1])

    def i_popframe(self, instruction):
        self.frames['TF'][-1] = self.frames['LF'][-1]
        self.frames['LF'].pop()

    def i_pushs(self, instruction):
        value = instruction[0]
        if value.attrib['type'] == 'var':
            l_frame, l_name = value.text.split('@')
            self.data_stack.append(self.frames[l_frame][-1][l_name])
        else:
            to_push = 0
            if value.attrib['type'] == 'int':
                to_push = int(value.text)
            elif value.attrib['type'] == 'bool':
                if value.text == 'true':
                    to_push = True
                elif value.text == 'false':
                    to_push = False
            elif value.attrib['type'] == 'string':
                to_push = value.text
            self.data_stack.append(to_push)

    def i_pops(self, instruction):
        var = instruction[0]
        frame, name = var.text.split('@')
        self.frames[frame][-1][name] = self.data_stack.pop()

    def i_move(self, instruction):
        var = instruction[0]
        value = instruction[1]
        frame, name = var.text.split('@')
        if value.attrib['type'] == 'var':
            l_frame, l_name = value.text.split('@')
            self.frames[frame][-1][name] = self.frames[l_frame][-1][l_name]
        else:
            if value.attrib['type'] == 'int':
                self.frames[frame][-1][name] = int(value.text)
            elif value.attrib['type'] == 'bool':
                if value.text == 'true':
                    self.frames[frame][-1][name] = True
                elif value.text == 'false':
                    self.frames[frame][-1][name] = False
            elif value.attrib['type'] == 'string':
                self.frames[frame][-1][name] = value.text

    def i_defvar(self, instruction):
        """ Vytvorenie a inicializacia premennej """
        var = instruction[0]
        frame, name = var.text.split('@')
        self.frames[frame][-1][name] = 0

    def i_return(self, instruction):
        self.pc = self.callbacks.pop()

    def i_call(self, instruction):
        name = instruction[0]
        if name.attrib['type'] == 'label':
            self.callbacks.append(self.pc)
            self.pc = self.labels[name.text]

    def i_add(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
        self.frames[frame][-1][name] = value_first + value_second

    def i_sub(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
        self.frames[frame][-1][name] = value_first - value_second

    def i_mul(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
        self.frames[frame][-1][name] = value_first * value_second

    def i_idiv(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
        self.frames[frame][-1][name] = value_first / value_second

    def i_write(self, instruction):
        value = instruction[0]
        if value.attrib['type'] == 'var':
            l_frame, l_name = value.text.split('@')
            value = self.frames[l_frame][-1][l_name]
            if value is True:
                print('true')
            elif value is False:
                print('false')
            else:
                print(value)
        else:
            print(value.text)

    def i_read(self, instruction):
        var = instruction[0]
        ttype = instruction[1]
        frame, name = var.text.split('@')
        value = input()
        if ttype.text == 'int':
            value = int(value)
        elif ttype.text == 'string':
            value = str(value)
        elif ttype.text == 'bool':
            if name == 'true':
                value = True
            elif name == 'false':
                value = False

        self.frames[frame][-1][name] = value

    def nop(self, instruction):
        pass

    def i_label(self, instruction):
        name = instruction[0]
        if name.attrib['type'] == 'label':
            self.labels[name.text] = self.pc

    def i_jump(self, instruction):
        name = instruction[0]
        if name.attrib['type'] == 'label':
            self.pc = self.labels[name.text]

    def i_jumpifneq(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        if var.attrib['type'] == 'label':
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'bool':
                if value1.text == 'true':
                    value_first = True
                elif value1.text == 'false':
                    value_second = False
            elif value1.attrib['type'] == 'string':
                value_first = value1.text
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]

            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'bool':
                if value2.text == 'true':
                    value_second = True
                elif value2.text == 'false':
                    value_second = False
            elif value2.attrib['type'] == 'string':
                value_second = value2.text
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
        result = value_first == value_second
        if result == False:
            self.pc = self.labels[var.text]

    def i_jumpifeq(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        if var.attrib['type'] == 'label':
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'bool':
                if value1.text == 'true':
                    value_first = True
                elif value1.text == 'false':
                    value_second = False
            elif value1.attrib['type'] == 'string':
                value_first = value1.text
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]

            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'bool':
                if value2.text == 'true':
                    value_second = True
                elif value2.text == 'false':
                    value_second = False
            elif value2.attrib['type'] == 'string':
                value_second = value2.text
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
        result = value_first == value_second
        if result == True:
            self.pc = self.labels[var.text]

    def i_dprint(self, instruction):
        value = instruction[0]
        if value.attrib['type'] == 'var':
            l_frame, l_name = value.text.split('@')
            value = self.frames[l_frame][-1][l_name]
            if value is True:
                print('true', file=sys.stderr)
            elif value is False:
                print('false', file=sys.stderr)
            else:
                print(value, file=sys.stderr)
        else:
            print(value.text, file=sys.stderr)

    def i_lt(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'bool':
                if value1.text == 'true':
                    value_first = True
                elif value1.text == 'false':
                    value_second = False
            elif value1.attrib['type'] == 'string':
                value_first = value1.text
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]

            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'bool':
                if value2.text == 'true':
                    value_second = True
                elif value2.text == 'false':
                    value_second = False
            elif value2.attrib['type'] == 'string':
                value_second = value2.text
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
        self.frames[frame][-1][name] = value_first < value_second

    def i_gt(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'bool':
                if value1.text == 'true':
                    value_first = True
                elif value1.text == 'false':
                    value_second = False
            elif value1.attrib['type'] == 'string':
                value_first = value1.text
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]

            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'bool':
                if value2.text == 'true':
                    value_second = True
                elif value2.text == 'false':
                    value_second = False
            elif value2.attrib['type'] == 'string':
                value_second = value2.text
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
        self.frames[frame][-1][name] = value_first > value_second

    def i_eq(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'bool':
                if value1.text == 'true':
                    value_first = True
                elif value1.text == 'false':
                    value_second = False
            elif value1.attrib['type'] == 'string':
                value_first = value1.text
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]

            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'bool':
                if value2.text == 'true':
                    value_second = True
                elif value2.text == 'false':
                    value_second = False
            elif value2.attrib['type'] == 'string':
                value_second = value2.text
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
        self.frames[frame][-1][name] = value_first == value_second

    def i_and(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'bool':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            if value2.attrib['type'] == 'bool':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
        self.frames[frame][-1][name] = value_first and value_second

    def i_or(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'bool':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            if value2.attrib['type'] == 'bool':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
        self.frames[frame][-1][name] = value_first or value_second

    def i_not(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'bool':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
        self.frames[frame][-1][name] = not value_first

    def i_int2char(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
        self.frames[frame][-1][name] = chr(value_first)

    def i_stri2int(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'string':
                value_first = value1.text
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
        self.frames[frame][-1][name] = ord(value_first[value_second])

    def i_concat(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'string':
                value_first = value1.text
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            if value2.attrib['type'] == 'string':
                value_second = value2.text
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
            self.frames[frame][-1][name] = value_first + value_second

    def i_strlen(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'string':
                value_first = value1.text
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
        self.frames[frame][-1][name] = len(value_first)

    def i_getchar(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'string':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
            self.frames[frame][-1][name] = value_first[value_second]

    def i_setchar(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            if value2.attrib['type'] == 'string':
                value_second = value2.text
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
            string = list(self.frames[frame][-1][name])
            string[value_first] = value_second[0]
            self.frames[frame][-1][name] = "".join(string)

    def i_type(self, instruction):
        var = instruction[0]
        value1 = instruction[1]
        frame, name = var.text.split('@')
        final_type = ""
        if var.attrib['type'] == 'var':
            if value1.attrib['type'] == 'bool':
                final_type = 'bool'
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
                final_type = str(type(value_first))
                if 'str' in final_type:
                    final_type = 'string'
                elif 'bool' in final_type:
                    final_type = 'bool'
                elif 'int' in final_type:
                    final_type = 'int'
            elif value1.attrib['type'] == 'int':
                final_type = 'int'
            elif value1.attrib['type'] == 'string':
                final_type = 'string'
        self.frames[frame][-1][name] = final_type

    def i_break(self, instruction):
        to_print = "State of interpretation\n"
        to_print += "Frames: {}\n".format(self.frames)
        to_print += "Program counter: {}\n".format(self.pc)
        to_print += "Stack: {}\n".format(self.data_stack)
        to_print += "Labels: {}\n".format(self.labels)
        print(to_print, file=sys.stderr)


def start_parsing(interpret, file=None):
    input_file = file or sys.stdin
    xml_tree = ElementTree.parse(input_file)
    root = xml_tree.getroot()

    for instruction in root:
        interpret.add_instruction(instruction)

    if len(root)!=0:
        interpret.load_labels()
        interpret.exec_program()
    return CODE_OK


def print_warning():
    print('Usage python interpret.py [--help] [--source]')


def check_params():
    if len(sys.argv) > 2:
        print('Bad arguments')
        sys.exit(WRONG_ARG)
    elif len(sys.argv) == 2:
        arg1 = sys.argv[1]
        if arg1 == '--help':
            print_warning()
            sys.exit(CODE_OK)
        elif arg1.startswith('--source='):
            return arg1[9:]
        else:
            print('Bad arguments')
            sys.exit(WRONG_ARG)
    return None


# Hlavny program
if __name__ == "__main__":
    filename = check_params()
    interpret = Interpret()
    if filename:
        start_parsing(interpret, file=filename)
    else:
        start_parsing(interpret)



