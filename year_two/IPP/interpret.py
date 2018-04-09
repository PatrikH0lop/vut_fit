"""
File: interpret.py
Author: Patrik Holop
About: Interpret of IPPcode18
"""


from xml.etree import ElementTree
import sys


class ErrorManagement:
    """Simple interface for error managing"""
    errors = {
        "wrong_arg": 10,
        "input_file": 11,
        "output_file": 12,
        "semantics": 52,
        "operand": 53,
        "not_defined": 54,
        "frame_not_exists": 55,
        "missing_value": 56,
        "zero_division": 57,
        "string_error": 58,
        "xml_syntax": 31,
        "xml_content": 32
    }

    @classmethod
    def raise_error(cls, code, message="Error"):
        """Prints warning and stops program"""
        print(message, file=sys.stderr)
        sys.exit(cls.errors[code])

    @classmethod
    def print_usage(cls):
        """Shows usage of program"""
        print('Usage python interpret.py [--help] [--source]')
        sys.exit(0)


class Interpret:
    """Basic interpret of IPPcode18"""
    def __init__(self):
        self.frames = {'GF': [{}], 'LF': [], 'TF': [None]}
        self.data_stack = []
        self.instructions = []
        self.labels = {}
        self.pc = 0
        self.callbacks = []
        self.frame_types = ["GF", "LF", "TF"]
        self.special_chars = ['_', '-', '$', '&', '%', '*']
        self.types = ["int", "bool", "float", "string"]
        self.controls = {
            "var": self.check_variable,
            "int": self.check_int,
            "bool": self.check_bool,
            "string": self.check_string,
            "float": self.check_float,
            "label": self.check_label,
            "type": self.check_type
        }
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
            "INT2FLOAT": self.i_int2float,
            "FLOAT2INT": self.i_float2int,
        }

    def add_instruction(self, instruction, counter):
        """Validates instruction shape and adds it to internal structure"""
        if instruction.tag == "instruction" and len(instruction.attrib) == 2:
            try:
                opcode = instruction.attrib['opcode']
                inst_order = int(instruction.attrib['order'])
                if opcode not in self.language_map or counter != inst_order:
                    ErrorManagement.raise_error("xml_content", "Content error in xml (counter or bad instr.)")
            except (KeyError, ValueError) as e:
                ErrorManagement.raise_error("xml_content", "Content error in xml (counter or bad instr.)")
        else:
            ErrorManagement.raise_error("xml_content", "Content error in xml (instruction)")
        self.instructions.append(instruction)

    def load_labels(self):
        """Creates internal list of labels before program execution"""
        self.pc = 0
        for instruction in self.instructions:
            opcode = instruction.attrib['opcode']
            if opcode == "LABEL":
                self.i_label(instruction)
            self.pc += 1
        self.pc = 0

    def exec_program(self):
        """Executes whole program"""
        while True:
            instruction = self.instructions[self.pc]
            self.exec_instruction(instruction)
            if self.pc >= len(self.instructions):
                sys.exit(0)

    def exec_instruction(self, instruction):
        """Recognizes and executes one instruction"""
        func = self.language_map.get(instruction.attrib['opcode'], "ERROR")
        func(instruction)
        self.pc += 1

    def check_int(self, text):
        """Valides the integer value"""
        if len(text) == 0:
            ErrorManagement.raise_error("xml_content", "No integer value found!")
        elif len(text) == 1 and text.isdigit() == False:
            ErrorManagement.raise_error("xml_content", "Wrong first character in integer")
        else:
            first_char = text[0]
            if first_char not in ["+", "-"] and not first_char.isdigit():
                ErrorManagement.raise_error("xml_content", "Wrong integer value!")
            for char in text[1:]:
                if char.isdigit() == False:
                    ErrorManagement.raise_error("xml_content", "Wrong integer value!")

    def check_float(self, text):
        """Valides the float value"""
        if len(text) == 0:
            ErrorManagement.raise_error("xml_content", "No float value found!")
        try:
            value = float.fromhex(text)
        except ValueError:
            ErrorManagement.raise_error("xml_content", "Bad float value found!")

    def check_string(self, text):
        """Valides the string value"""
        if text == "":
            return
        state = "char"
        esc_count = 0
        for char in text:
            if state == "char":
                if char.isspace() or char == "#":
                    ErrorManagement.raise_error("xml_content", "Wrong string value")
                if char == "\\":
                    state = "esc"
            elif state == "esc":
                if char.isdigit():
                    esc_count += 1
                else:
                    ErrorManagement.raise_error("xml_content", "Wrong escape sequence in string!")
                if esc_count == 3:
                    esc_count = 0
                    state = "char"

    def check_bool(self, text):
        """Valides the bool value"""
        if text not in ["true", "false"]:
            ErrorManagement.raise_error("xml_content", "Wrong boolean value found!")

    def check_type(self, text):
        """Checks validity of type"""
        if text not in self.types:
            ErrorManagement.raise_error("xml_content", "Nonexisting type found!")

    def check_label(self, text):
        """Checks validity of label name"""
        if len(text) == 0:
            ErrorManagement.raise_error("xml_content", "Wrong name for label")
        first_char = text[0]
        if first_char not in self.special_chars and not first_char.isalpha():
            ErrorManagement.raise_error("operand", "Wrong first char in label name")
        for char in text[1:]:
            if char.isspace():
                raise ErrorManagement("operand", "Wrong char in label name, whitespace found")
            elif not char.isalpha() and not char.isdigit() and char not in self.special_chars:
                raise ErrorManagement("operand", "Wrong char in label name, unknown symbol found")

    def check_variable(self, argument):
        """Checks variable
        :returns (frame, name)
        """
        if len(argument) <= 3:
            ErrorManagement.raise_error("operand", "Wrong variable shape, no content")
        v_type = argument[:2]
        if argument[2] != "@":
            ErrorManagement.raise_error("operand", "Wrong variable shape, no @")
        if v_type not in self.frame_types:
            ErrorManagement.raise_error("operand", "Wrong variable shape, unknown type")
        name = argument[3:]
        first_char = name[0]
        if first_char not in self.special_chars and not first_char.isalpha():
            ErrorManagement.raise_error("operand", "Wrong first char in variable name")
        for char in name[1:]:
            if char.isspace():
                raise ErrorManagement("operand", "Wrong char in variable name, whitespace found")
            elif not char.isalpha() and not char.isdigit() and char not in self.special_chars:
                raise ErrorManagement("operand", "Wrong char in variable name, unknown symbol found")

    def check_argument(self, argument, order, expected_type):
        """Checks one argument"""
        #print(argument, order, expected_type)
        if argument.tag != "arg" + str(order) or len(argument) != 0:
            ErrorManagement.raise_error("xml_content", "Wrong shape of argument")
        if len(argument.attrib) != 1:
            ErrorManagement.raise_error("xml_content", "Wrong shape of argument")
        try:
            v_type = argument.attrib['type']
            if expected_type == "symb":
                possibilities = self.types + ["var"]
            else:
                possibilities = [expected_type]
            if v_type not in possibilities:
                ErrorManagement.raise_error("operand", "Wrong type of argument")
            check_function = self.controls[v_type]
            if v_type == "string":
                if argument.text is None:
                    argument.text = ""
                else:
                    ret = check_function(argument.text)
            else:
                check_function(argument.text)
        except KeyError:
            ErrorManagement.raise_error("xml_content", "Wrong shape of argument")

    def check_arguments(self, instruction, expected_args):
        """Checks all arguments of instruction"""
        if len(instruction) != len(expected_args):
            ErrorManagement.raise_error("xml_content", "Wrong number of params in instr. {}".format(instruction.attrib['opcode']))
        counter = 1
        for argument, expected_type in zip(instruction, expected_args):
            self.check_argument(argument, counter, expected_type)
            counter += 1

    def print_stacks(self):
        """Debug function to print content of stacks"""
        print('GF:', self.frames['GF'], file=sys.stderr)
        print('LF:', self.frames['LF'], file=sys.stderr)
        print('TF:', self.frames['TF'], file=sys.stderr)

    def detype_value(self, _type, text):
        """Return value in python representation"""
        try:
            if _type == 'int':
                return int(text)
            elif _type == 'bool':
                if text == 'true':
                    return True
                elif text == 'false':
                    return False
            elif _type == 'string':
                return text
                #TODO
            elif _type == 'float':
                return float.fromhex(text)
        except ValueError:
            ErrorManagement.raise_error("operand", "Wrong variable type!")

    def i_move(self, instruction):
        """Stores the given value in variable"""
        self.check_arguments(instruction, ["var", "symb"])
        var = instruction[0]
        value = instruction[1]
        var_frame, var_name = var.text.split('@')
        try:
            if var_name not in self.frames[var_frame][-1]:
                ErrorManagement.raise_error("not_defined", "Not existing variable")
            if value.attrib['type'] == 'var':
                l_frame, l_name = value.text.split('@')
                self.frames[var_frame][-1][var_name] = self.frames[l_frame][-1][l_name]
            else:
                self.frames[var_frame][-1][var_name] = self.detype_value(value.attrib['type'], value.text)
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def i_createframe(self, instruction):
        """Creates new temporary frame and removes the existing one"""
        self.check_arguments(instruction, [])
        self.frames['TF'][-1] = {}

    def i_pushframe(self, instruction):
        """Moves temporary frames to local frame stack"""
        self.check_arguments(instruction, [])
        if self.frames['TF'][-1] is None:
            ErrorManagement.raise_error("frame_not_exists", "TF is in undefined state!")
        self.frames['LF'].append(self.frames['TF'][-1])
        self.frames['TF'][-1] = None

    def i_popframe(self, instruction):
        """Moves last local frame from stack to temporary frame"""
        self.check_arguments(instruction, [])
        if not self.frames['LF']:
            ErrorManagement.raise_error("frame_not_exists", "LF is in undefined state!")
        self.frames['TF'][-1] = self.frames['LF'][-1]
        self.frames['LF'].pop()

    def i_defvar(self, instruction):
        """Creates new variable on the given frame"""
        self.check_arguments(instruction, ["var"])
        var = instruction[0]
        frame, name = var.text.split('@')
        try:
            if name in self.frames[frame][-1]:
                ErrorManagement.raise_error("frame_not_exists", "Error, redefining variable")
            self.frames[frame][-1][name] = 0
        except (TypeError, IndexError) as e:
            ErrorManagement.raise_error("frame_not_exists", "Error while creating new variable")

    def i_return(self, instruction):
        """Returns instruction pointer to point when function was called"""
        self.check_arguments(instruction, [])
        if len(self.callbacks) > 0:
            self.pc = self.callbacks.pop()
        else:
            ErrorManagement.raise_error("missing_value", "No place to return")

    def i_call(self, instruction):
        """Calls the function by jump to label"""
        self.check_arguments(instruction, ["label"])
        name = instruction[0]
        if name.text in self.labels:
            self.callbacks.append(self.pc)
            self.pc = self.labels[name.text]
        else:
            ErrorManagement.raise_error("semantics", "No label {} to call".format(name.text))

    def i_pushs(self, instruction):
        """Push value to data stack"""
        self.check_arguments(instruction, ["symb"])
        value = instruction[0]
        try:
            if value.attrib['type'] == 'var':
                l_frame, l_name = value.text.split('@')
                self.data_stack.append(self.frames[l_frame][-1][l_name])
            else:
                to_push = self.detype_value(value.attrib['type'], value.text)
                self.data_stack.append(to_push)
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def i_pops(self, instruction):
        """Pops value from stack"""
        self.check_arguments(instruction, ["var"])
        var = instruction[0]
        frame, name = var.text.split('@')
        try:
            try:
                value = self.data_stack.pop()
            except IndexError:
                ErrorManagement.raise_error("missing_value", "Pop from empty stack!")
            self.frames[frame][-1][name] = value
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def arithmetic_precheck(self, value1, value2):
        """Performs arithmetic precheck
        :returns 2 values for arithmetic instructions
        """
        try:
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'float':
                value_first = float.fromhex(value1.text)
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            else:
                ErrorManagement.raise_error("operand", "Wrong operand type!")
            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'float':
                value_second = float.fromhex(value2.text)
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
            else:
                ErrorManagement.raise_error("operand", "Wrong operand type!")
            return value_first, value_second
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")
        except ValueError:
            ErrorManagement.raise_error("operand", "Wrong variable type!")

    def i_add(self, instruction):
        """Adds 2 values and result saves in the variable"""
        self.check_arguments(instruction, ["var", "symb", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        try:
            value_first, value_second = self.arithmetic_precheck(value1, value2)
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            self.frames[frame][-1][name] = value_first + value_second
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def i_sub(self, instruction):
        """Subs 2 values and result saves in the variable"""
        self.check_arguments(instruction, ["var", "symb", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        try:
            value_first, value_second = self.arithmetic_precheck(value1, value2)
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            self.frames[frame][-1][name] = value_first - value_second
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def i_mul(self, instruction):
        """Multiplies 2 values and result saves in the variable"""
        self.check_arguments(instruction, ["var", "symb", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        try:
            value_first, value_second = self.arithmetic_precheck(value1, value2)
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            self.frames[frame][-1][name] = value_first * value_second
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def i_idiv(self, instruction):
        """Divides 2 values and result saves in the variable"""
        self.check_arguments(instruction, ["var", "symb", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        try:
            value_first, value_second = self.arithmetic_precheck(value1, value2)
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            if value_second == 0:
                ErrorManagement.raise_error("zero_division", "Division by zero")
            self.frames[frame][-1][name] = value_first // value_second
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def value_to_text(self, value):
        """Converts value to output string"""
        if isinstance(value, float):
            return str(value.hex())
        if value is True:
            return 'true'
        elif value is False:
            return 'false'
        else:
            return str(value)

    def i_write(self, instruction):
        """Writes symbol to standart output"""
        self.check_arguments(instruction, ["symb"])
        value = instruction[0]
        try:
            if value.attrib['type'] == 'var':
                l_frame, l_name = value.text.split('@')
                value = self.frames[l_frame][-1][l_name]
                to_print = self.value_to_text(value)
            else:
                value = self.detype_value(value.attrib['type'], value.text)
                to_print = self.value_to_text(value)
            state = "char"
            escape_sequences = {}
            esc = ""
            esc_count = 0
            for char in to_print:
                if state == "char":
                    if char == "\\":
                        state = "esc"
                elif state == "esc":
                    if char.isdigit():
                        esc += char
                        esc_count += 1
                    if esc_count == 3:
                        escape_sequences[esc] = chr(int(esc))
                        esc = ""
                        esc_count = 0
                        state = "char"
            for esc in escape_sequences:
                to_print = to_print.replace("\\" + esc, escape_sequences[esc])
            print(to_print)
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def i_read(self, instruction):
        """Reads value from stdin"""
        self.check_arguments(instruction, ["var", "type"])
        var = instruction[0]
        _type = instruction[1]
        frame, name = var.text.split('@')
        value = input()
        try:
            value = self.detype_value(_type.text, value)
            self.frames[frame][-1][name] = value
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def nop(self, instruction):
        """Does nothing, successfully"""
        pass

    def i_label(self, instruction):
        """Defines new label"""
        self.check_arguments(instruction, ["label"])
        name = instruction[0]
        if name.text in self.labels:
            ErrorManagement.raise_error("missing_value", "Redefinition of label")
        self.labels[name.text] = self.pc

    def i_jump(self, instruction):
        """Jumps to given label"""
        self.check_arguments(instruction, ["label"])
        name = instruction[0]
        try:
            self.pc = self.labels[name.text]
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Label does not exist!")

    def logical_precheck(self, value1, value2):
        """Performs arithmetic precheck
        :returns 2 values for arithmetic instructions
        """
        try:
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'float':
                value_first = float.fromhex(value1.text)
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            elif value1.attrib['type'] == 'string':
                value_first = value1.text
            elif value1.attrib['type'] == 'bool':
                if value1.text == 'true':
                    value_first = True
                elif value1.text == 'false':
                    value_first = False
            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'float':
                value_second = float.fromhex(value2.text)
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
            elif value2.attrib['type'] == 'string':
                value_second = value2.text
            elif value2.attrib['type'] == 'bool':
                if value2.text == 'true':
                    value_second = True
                elif value2.text == 'false':
                    value_second = False
            return value_first, value_second
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")
        except ValueError:
            ErrorManagement.raise_error("operand", "Wrong variable type!")

    def i_jumpifneq(self, instruction):
        """Jumps to given label if values are not equal"""
        self.check_arguments(instruction, ["label", "symb", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        value_first, value_second = self.logical_precheck(value1, value2)
        result = value_first == value_second
        try:
            if result == False:
                self.pc = self.labels[var.text]
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Label does not exist!")

    def i_jumpifeq(self, instruction):
        """Jumps to given label if values are equal"""
        self.check_arguments(instruction, ["label", "symb", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        value_first, value_second = self.logical_precheck(value1, value2)
        result = value_first == value_second
        try:
            if result == True:
                self.pc = self.labels[var.text]
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Label does not exist!")

    def i_dprint(self, instruction):
        """Writes symbol to standart output"""
        self.check_arguments(instruction, ["symb"])
        value = instruction[0]
        try:
            if value.attrib['type'] == 'var':
                l_frame, l_name = value.text.split('@')
                value = self.frames[l_frame][-1][l_name]
                to_print = self.value_to_text(value)
            else:
                value = self.detype_value(value.attrib['type'], value.text)
                to_print = self.value_to_text(value)
            print(to_print, file=sys.stderr)
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def i_lt(self, instruction):
        """Compare 2 values and returns True if first is lower"""
        self.check_arguments(instruction, ["var", "symb", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        try:
            if value1.attrib['type'] != "var" and value2.attrib['type'] != "var":
                if value1.attrib['type'] != value2.attrib['type']:
                    ErrorManagement.raise_error("semantics")
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            value_first, value_second = self.logical_precheck(value1, value2)
            self.frames[frame][-1][name] = value_first < value_second
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def i_gt(self, instruction):
        """Compare 2 values and returns True if first is greater"""
        self.check_arguments(instruction, ["var", "symb", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        try:
            if value1.attrib['type'] != "var" and value2.attrib['type'] != "var":
                if value1.attrib['type'] != value2.attrib['type']:
                    ErrorManagement.raise_error("semantics")
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            value_first, value_second = self.logical_precheck(value1, value2)
            self.frames[frame][-1][name] = value_first > value_second
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def i_eq(self, instruction):
        """Compare 2 values and returns True if they are equal"""
        self.check_arguments(instruction, ["var", "symb", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        try:
            if value1.attrib['type'] != "var" and value2.attrib['type'] != "var":
                if value1.attrib['type'] != value2.attrib['type']:
                    ErrorManagement.raise_error("semantics")
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            value_first, value_second = self.logical_precheck(value1, value2)
            self.frames[frame][-1][name] = value_first == value_second
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def boolean_precheck(self, value1, value2):
        """Prechecks 2 boolean values"""
        try:
            if value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            elif value1.attrib['type'] == 'bool':
                if value1.text == 'true':
                    value_first = True
                elif value1.text == 'false':
                    value_first = False
            else:
                ErrorManagement.raise_error("operand", "Expected boolean value!")

            if value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
            elif value2.attrib['type'] == 'bool':
                if value2.text == 'true':
                    value_second = True
                elif value2.text == 'false':
                    value_second = False
            else:
                ErrorManagement.raise_error("operand", "Expected boolean value!")
            return value_first, value_second
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")
        except ValueError:
            ErrorManagement.raise_error("operand", "Wrong variable type!")

    def i_and(self, instruction):
        """Logical and over 2 values"""
        self.check_arguments(instruction, ["var", "symb", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        try:
            value_first, value_second = self.boolean_precheck(value1, value2)
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            self.frames[frame][-1][name] = value_first and value_second
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def i_or(self, instruction):
        """Logical or over 2 values"""
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        try:
            value_first, value_second = self.boolean_precheck(value1, value2)
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            self.frames[frame][-1][name] = value_first or value_second
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def i_not(self, instruction):
        """Negates boolean value"""
        self.check_arguments(instruction, ["var", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        frame, name = var.text.split('@')
        try:
            if value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            elif value1.attrib['type'] == 'bool':
                if value1.text == 'true':
                    value_first = True
                elif value1.text == 'false':
                    value_first = False
                else:
                    ErrorManagement.raise_error("operand", "Expected boolean value!")
            else:
                ErrorManagement.raise_error("operand", "Expected boolean value!")
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            self.frames[frame][-1][name] = not value_first
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")
        except ValueError:
            ErrorManagement.raise_error("operand", "Wrong variable type!")

    def i_int2char(self, instruction):
        """Converts integer to char value"""
        self.check_arguments(instruction, ["var", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        frame, name = var.text.split('@')
        try:
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            else:
                ErrorManagement.raise_error("operand", "Wrong variable type!")
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            self.frames[frame][-1][name] = chr(value_first)
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")
        except ValueError:
            ErrorManagement.raise_error("operand", "Wrong variable type!")

    def i_stri2int(self, instruction):
        """Converts string to integer"""
        self.check_arguments(instruction, ["var", "symb", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        try:
            if value1.attrib['type'] == 'string':
                value_first = value1.text
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            else:
                ErrorManagement.raise_error("operand", "Wrong operand type")
            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
            else:
                ErrorManagement.raise_error("operand", "Wrong operand type")
            try:
                value = ord(value_first[value_second])
            except IndexError:
                ErrorManagement.raise_error("string_error", "Wrong index of paramater")
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            self.frames[frame][-1][name] = value
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")
        except ValueError:
            ErrorManagement.raise_error("operand", "Wrong variable type!")


    def i_float2int(self, instruction):
        """TODO"""
        pass

    def i_int2float(self, instruction):
        """TODO"""
        pass

    def i_concat(self, instruction):
        """Concatenates 2 string values"""
        self.check_arguments(instruction, ["var", "symb", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        try:
            if value1.attrib['type'] == 'string':
                value_first = value1.text
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            else:
                ErrorManagement.raise_error("operand", "Wrong variable type")
            if value2.attrib['type'] == 'string':
                value_second = value2.text
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
            else:
                ErrorManagement.raise_error("operand", "Wrong variable type")
            try:
                final_value = value_first + value_second
            except TypeError:
                ErrorManagement.raise_error("operand", "Wrong variable type")
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            self.frames[frame][-1][name] = final_value
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")
        except ValueError:
            ErrorManagement.raise_error("operand", "Wrong variable type!")

    def i_strlen(self, instruction):
        """Calculates the length of string"""
        var = instruction[0]
        value1 = instruction[1]
        frame, name = var.text.split('@')
        try:
            if value1.attrib['type'] == 'string':
                value_first = value1.text
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            else:
                ErrorManagement.raise_error("operand", "Wrong operand type")
            try:
                length = len(value_first)
            except TypeError:
                ErrorManagement.raise_error("operand", "Wrong operand")
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            self.frames[frame][-1][name] = length
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")

    def i_getchar(self, instruction):
        """Return char at index in string"""
        self.check_arguments(instruction, ["var", "symb", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        try:
            if value1.attrib['type'] == 'string':
                value_first = value1.text
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            else:
                ErrorManagement.raise_error("operand", "Wrong operand type")
            if value2.attrib['type'] == 'int':
                value_second = int(value2.text)
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
            else:
                ErrorManagement.raise_error("operand", "Wrong operand type")
            try:
                value = value_first[value_second]
            except IndexError:
                ErrorManagement.raise_error("string_error", "Wrong indexing in string!")
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            self.frames[frame][-1][name] = value
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")
        except ValueError:
            ErrorManagement.raise_error("operand", "Wrong variable type!")

    def i_setchar(self, instruction):
        """Sets specific char at index in string"""
        self.check_arguments(instruction, ["var", "symb", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        value2 = instruction[2]
        frame, name = var.text.split('@')
        try:
            if value1.attrib['type'] == 'int':
                value_first = int(value1.text)
            elif value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
            else:
                ErrorManagement.raise_error("operand", "Wrong operand type")
            if value2.attrib['type'] == 'string':
                value_second = value2.text
            elif value2.attrib['type'] == 'var':
                lframe, lname = value2.text.split('@')
                value_second = self.frames[lframe][-1][lname]
            else:
                ErrorManagement.raise_error("operand", "Wrong operand type")
            string = list(self.frames[frame][-1][name])
            try:
                string[value_first] = value_second[0]
            except IndexError:
                ErrorManagement.raise_error("string_error", "Wrong indexing in char")
            self.frames[frame][-1][name] = "".join(string)
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")
        except ValueError:
            ErrorManagement.raise_error("operand", "Wrong variable type!")

    def i_type(self, instruction):
        """Checks type of value dynamically"""
        self.check_arguments(instruction, ["var", "symb"])
        var = instruction[0]
        value1 = instruction[1]
        frame, name = var.text.split('@')
        final_type = ""
        try:
            if value1.attrib['type'] == 'var':
                lframe, lname = value1.text.split('@')
                value_first = self.frames[lframe][-1][lname]
                final_type = str(type(value_first))
                if 'str' in final_type:
                    final_type = 'string'
                elif 'bool' in final_type:
                    final_type = 'bool'
                elif 'int' in final_type:
                    final_type = 'int'
                elif 'float' in final_type:
                    final_type = 'float'
                else:
                    ErrorManagement.raise_error("operand", "Unknown type!")
            elif value1.attrib['type'] == 'bool':
                final_type = 'bool'
            elif value1.attrib['type'] == 'int':
                final_type = 'int'
            elif value1.attrib['type'] == 'string':
                final_type = 'string'
            elif value1.attrib['type'] == 'float':
                final_type = 'float'
            if name not in self.frames[frame][-1]:
                ErrorManagement.raise_error("not_defined", "Variable does not exists!")
            self.frames[frame][-1][name] = final_type
        except (TypeError, IndexError):
            ErrorManagement.raise_error("frame_not_exists", "Frame does not exist!")
        except KeyError:
            ErrorManagement.raise_error("not_defined", "Variable does not exist!")
        except ValueError:
            ErrorManagement.raise_error("operand", "Wrong variable type!")

    def i_break(self, instruction):
        """Prints debug information to stderr"""
        to_print = "State of interpretation\n"
        to_print += "Frames: {}\n".format(self.frames)
        to_print += "Program counter: {}\n".format(self.pc)
        to_print += "Stack: {}\n".format(self.data_stack)
        to_print += "Labels: {}\n".format(self.labels)
        print(to_print, file=sys.stderr)


def start_parsing(file):
    """Parses input file or stdin and runs interpret"""
    input_file = file or sys.stdin
    try:
        xml_tree = ElementTree.parse(input_file)
    except FileNotFoundError:
        ErrorManagement.raise_error("input_file", "Wrong source file name")
    except ElementTree.ParseError:
        ErrorManagement.raise_error("xml_syntax", "Wrong xml structure")

    root = xml_tree.getroot()
    allowed = ["language", "name", "description"]

    if root.tag != "program" or len(root.attrib) > 3:
        ErrorManagement.raise_error("xml_content", "Wrong content of xml (program)")
    try:
        for attrib in root.attrib:
            if attrib not in allowed:
                ErrorManagement.raise_error("xml_content", "Wrong content of xml (program)")
        lang = root.attrib["language"]
        if lang != "IPPcode18":
            ErrorManagement.raise_error("xml_content", "Wrong content of xml (language)")
    except KeyError:
        ErrorManagement.raise_error("xml_content", "Wrong content of xml (language)")

    interpret = Interpret()

    counter = 1
    for instruction in root:
        interpret.add_instruction(instruction, counter)
        counter += 1

    if len(root) != 0:
        interpret.load_labels()
        interpret.exec_program()


def check_params():
    """Checks correct parameters"""
    if len(sys.argv) > 2:
        ErrorManagement.raise_error("wrong_arg", "Bad arguments")
    elif len(sys.argv) == 2:
        arg1 = sys.argv[1]
        if arg1 == '--help':
            ErrorManagement.print_usage()
        elif arg1.startswith('--source='):
            if len(arg1) <= 9:
                ErrorManagement.raise_error("input_file", "Wrong source file name")
            return arg1[9:]
        else:
            ErrorManagement.raise_error("wrong_arg", "Bad arguments")
    return None


#MAIN FILE
if __name__ == "__main__":
    filename = check_params()
    start_parsing(file=filename)



