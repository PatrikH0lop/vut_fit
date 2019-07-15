<?php
#Author: xholop01
#IPP1 : IPPcode18 -> XML


#Konstanty pre typy objektov
define("VARIABLE", 0);
define("SYMB", 1);
define("LABEL", 2);
define("TTYPE", 3);

#Programove konstanty
define("NAME_ERROR", false);
define("WRONG_PARAM", 10);
define("LS_ERROR", 21);
define("CODE_OK", 0);
define("FILE_ERROR", 12);


#Instrukcie jazyka IPPcode18
$instructions = array(
    "MOVE" => array(VARIABLE, SYMB),
    "CREATEFRAME" => array(),
    "PUSHFRAME" => array(),
    "POPFRAME" => array(),
    "DEFVAR" => array(VARIABLE),
    "CALL" => array(LABEL),
    "RETURN" => array(),
    "PUSHS" => array(SYMB),
    "POPS" => array(VARIABLE),
    "ADD" => array(VARIABLE, SYMB, SYMB),
    "SUB" => array(VARIABLE, SYMB, SYMB),
    "MUL" => array(VARIABLE, SYMB, SYMB),
    "IDIV" => array(VARIABLE, SYMB, SYMB),
    "LT" => array(VARIABLE, SYMB, SYMB),
    "GT" => array(VARIABLE, SYMB, SYMB),
    "EQ" => array(VARIABLE, SYMB, SYMB),
    "AND" => array(VARIABLE, SYMB, SYMB),
    "OR" => array(VARIABLE, SYMB, SYMB),
    "NOT" => array(VARIABLE, SYMB),
    "INT2CHAR" => array(VARIABLE, SYMB),
    "STRI2INT" => array(VARIABLE, SYMB, SYMB),
    "READ" => array(VARIABLE, TTYPE),
    "WRITE" => array(SYMB),
    "CONCAT" => array(VARIABLE, SYMB, SYMB),
    "STRLEN" => array(VARIABLE, SYMB),
    "GETCHAR" => array(VARIABLE, SYMB, SYMB),
    "SETCHAR" => array(VARIABLE, SYMB, SYMB),
    "TYPE" => array(VARIABLE, SYMB),
    "LABEL" => array(LABEL),
    "JUMP" => array(LABEL),
    "JUMPIFEQ" => array(LABEL, SYMB, SYMB),
    "JUMPIFNEQ" => array(LABEL, SYMB, SYMB),
    "DPRINT" => array(SYMB),
    "BREAK" => array()
);


#Vypis varovania o zlych argumentoch
function print_warning() {
    fwrite(STDERR, "Bad arguments, run script with --help\n");
}


#Kontrola parametrov
function check_params($argv, &$stats, &$loc_first, &$loc, &$comments, &$file) {
    if (count($argv)>1) {
        $help = false;
        $stats = false;
        $loc = false;
        $comments = false;
        $loc_first = false;

        foreach (array_slice($argv, 1) as $arg) {
            switch ($arg) {
                case "--help":
                    if ($help == false) {
                        $help = true;
                    } else {
                        print_warning();
                        exit(WRONG_PARAM);
                    }
                    break;
                case "--loc":
                    if ($loc == false) {
                        $loc_first = false;
                        $loc = true;
                    } else {
                        print_warning();
                        exit(WRONG_PARAM);
                    }
                    break;
                case "--comments":
                    if ($comments == false) {
                        $loc_first = true;
                        $comments = true;
                    } else {
                        print_warning();
                        exit(WRONG_PARAM);
                    }
                    break;
                default:
                    if ($stats == false) {
                        if (strpos($arg, "--stats=") === 0) {
                            $file = substr($arg, 8);
                            if ($file == "") {
                                print_warning();
                                exit(FILE_ERROR);
                            }
                            $stats = true;
                        } else {
                            print_warning();
                            exit(WRONG_PARAM);
                        }
                    } else {
                        print_warning();
                        exit(WRONG_PARAM);
                    }
                    break;
            }
        }
        if ($help === true and $loc === false and $comments === false and $stats === $comments) {
            echo "Usage: php5.6 parse.php [--help] [--stats [--loc|--comments]]\n";
            exit(CODE_OK);
        } else if ($help === true) {
            print_warning();
            exit(WRONG_PARAM);
        } else if (($stats === false and ($loc === true or $comments === true))
                    or ($stats === true and ($loc === false and $comments === false))) {
            print_warning();
            exit(WRONG_PARAM);
        }
    }
}


#Overi meno premennej
function check_variable_name($id) {
    $symbols = array('_', '-', '$', '&', '%', '*'); #specialne znaky
    if (strlen($id) == 1) { #jednopismenny nazov
        if (in_array($id, $symbols) or ctype_alpha($id)) {
            return CODE_OK;
        }
        return NAME_ERROR;
    }
    elseif (strlen($id) > 1) { #viacpismenny tvar
        $ret = CODE_OK; #navratova hodnota
        if (!in_array($id[0], $symbols) and ctype_alpha($id[0])!=true) {
            $ret = NAME_ERROR;
        }
        foreach (str_split($id) as $value) {
            if (!ctype_alnum($value) and !in_array($value, $symbols)) {
                $ret = NAME_ERROR;
            }
        }

        return $ret;
    }
    return CODE_OK;
}


#Overi znenie konstanty
function check_constant($ttype, $value) {

    switch ($ttype) {

        case "int": #kontrola cisiel
            if (is_numeric($value) and strpos($value, ".") === false and strpos($value, "e") === false) {
                return $value;
            }
            break;

        case "bool":
            if ($value == "true" or $value == "false") {
                return $value;
            }
            break;

        case "string":
            $state = "char";
            $esc_count = 0;
            if ($value === "")
                return $value;
            foreach (str_split($value) as $char) {
                if ($state == "char") {
                    if (!ctype_space($char) and $char!="#") {
                        if ($char == "\\") {
                            $state = "esc";
                            $esc_count = 0;
                        }
                    }
                    else {
                        return NAME_ERROR;
                    }
                }
                else if ($state == "esc") {
                    if (is_numeric($char)) {
                        $esc_count += 1;
                    } else {
                        return NAME_ERROR;
                    }
                    if ($esc_count == 3) {
                        $esc_count = 0;
                        $state = "char";
                    }
                }
            }
            if ($state == "esc") {
                return NAME_ERROR;
            }
            return $value;
            break;
    }

    return NAME_ERROR;
}


#zisti typ pre argumenty
#opcode -> operacny kod napr. DEFVAR
#arg -> argument zo vstupu
#order -> poradie argumentu
#VRACIA tuple(typ, meno|hodnota)
function check_arg($opcode, $arg, $order) {
    global $instructions;
    $arguments = $instructions[$opcode];
    $ttype = $arguments[$order];

    #ocakavany typ argumentu
    switch ($ttype) {

        #tvar ramec@meno
        case VARIABLE:
            if (substr_count($arg, '@') == 1) { #obsahuje 1 @
                $divided = explode("@", $arg); #rozdelenie na ramec a meno
                if ($divided[0] == "TF" || $divided[0] == "GF" || $divided[0] == "LF") {
                    $name = check_variable_name($divided[1]);
                    if ($name !== NAME_ERROR) {
                        $divided[1] = str_replace("&", "&amp;", $divided[1]);
                        return array("var", strtoupper($divided[0])."@".$divided[1]);
                    }
                }
            }
            break;

        #tvar ako meno premennej
        case LABEL:
            $name = check_variable_name($arg);
            if ($name !== NAME_ERROR) {
                $arg = str_replace("&", "&amp;", $arg);
                return array("label", $arg);
            }
            break;

        #konstanta alebo premenna, tvar konstanty typ@hodnota
        case SYMB:
            #argument je konstanta
            $types = array("int", "bool", "string");
            $is_string = false;
            if (substr($arg, 0, 6) === "string") {
                $is_string = true;
            }
            #var_dump($is_string);
            if (substr_count($arg, '@') == 1) { #obsahuje 1 @
                $divided = explode("@", $arg); #rozdelenie na typ a hodnotu
                if (in_array($divided[0], $types)) {
                    $name = check_constant($divided[0], $divided[1]);
                    if ($name !== NAME_ERROR) {
                        $name = str_replace("&", "&amp;", $name);
                        return array($divided[0], $name);
                    }
                }
            } else if ($is_string == true) {
                $divided = explode("@", $arg); #rozdelenie na typ a hodnotu
                $opp = array_shift($divided);
                $val = join("@", $divided);
                #echo $opp . "->>" . $val;
                if (in_array($opp, $types)) {
                    $name = check_constant($opp, $val);
                    if ($name !== NAME_ERROR) {
                        $name = str_replace("&", "&amp;", $name);
                        return array($opp, $name);
                    }
                }
            }
            #argument je premenna
            if (substr_count($arg, '@') == 1) {
                $divided = explode("@", $arg);
                if ($divided[0] == "TF" || $divided[0] == "GF" || $divided[0] == "LF") {
                    $name = check_variable_name($divided[1]);
                    if ($name !== NAME_ERROR) {
                        $divided[1] = str_replace("&", "&amp;", $divided[1]);
                        return array("var", strtoupper($divided[0])."@".$divided[1]);
                    }
                }
            }
            break;

        #tvar int, bool, string
        case TTYPE:
            $types = array("int", "bool", "string");
            if (in_array($arg, $types)) {
                return array("type", $arg);
            }
            break;
    }

    return NAME_ERROR;
}


#Funkcia na spracovanie instrukcie
#line -> riadok zo vstupu
#order -> poriadie instrukcie
#xml -> vytvarany xml
function recognize_inst($line, $order, $xml) {

    #remove ' ' and '\t' from string
    $reg_line = preg_replace('/\s+/', ' ', $line);
    $trimmed = trim($reg_line);
    $exploded = explode(' ', $trimmed);
    $opcode = strtoupper($exploded[0]);

    #kontrola instrukcie
    global $instructions;
    if (array_key_exists($opcode, $instructions) === false) { #instrukcia nie je v jazyku
        fwrite(STDERR, "Not valid instruction in language IPPcode18\n");
        return LS_ERROR;
    }
    else if (count($instructions[$opcode]) != count($exploded)-1) { #zly pocet parametrov
        #echo $line;
        fwrite(STDERR, "Instruction uses different number of arguments\n");
        return LS_ERROR;
    }

    #vygenerovanie instrukcie v xml
    $command = $xml->addChild("instruction");
    $command->addAttribute("order", $order);
    $command->addAttribute("opcode", $opcode);


    #1 parameter
    if (count($exploded) == 2) {
        $info = check_arg($opcode, $exploded[1], 0);
        if ($info == NAME_ERROR) {
            fwrite(STDERR, "Wrong name of first argument\n");
            return LS_ERROR;
        }
        $arg1 = $command->addChild("arg1", $info[1]);
        $arg1->addAttribute("type", $info[0]);
    }
    #2 parametre
    else if (count($exploded) == 3) {
        $info = check_arg($opcode, $exploded[1], 0);
        if ($info == NAME_ERROR) {
            fwrite(STDERR, "Wrong name of first argument\n");
            return LS_ERROR;
        }
        $arg1 = $command->addChild("arg1", $info[1]);
        $arg1->addAttribute("type", $info[0]);

        $info = check_arg($opcode, $exploded[2], 1);
        if ($info == NAME_ERROR) {
            fwrite(STDERR, "Wrong name of second argument\n");
            return LS_ERROR;
        }

        $arg2 = $command->addChild("arg2", $info[1]);
        $arg2->addAttribute("type", $info[0]);
    }
    #3 parametre
    else if (count($exploded) == 4) {
        $info = check_arg($opcode, $exploded[1], 0);
        if ($info == NAME_ERROR) {
            fwrite(STDERR, "Wrong name of first argument\n");
            return LS_ERROR;
        }
        $arg1 = $command->addChild("arg1", $info[1]);
        $arg1->addAttribute("type", $info[0]);

        $info = check_arg($opcode, $exploded[2], 1);
        if ($info == NAME_ERROR) {
            fwrite(STDERR, "Wrong name of second argument\n");
            return LS_ERROR;
        }
        $arg1 = $command->addChild("arg2", $info[1]);
        $arg1->addAttribute("type", $info[0]);

        $info = check_arg($opcode, $exploded[3], 2);
        if ($info == NAME_ERROR) {
            fwrite(STDERR, "Wrong name of third argument\n");
            return LS_ERROR;
        }
        $arg1 = $command->addChild("arg3", $info[1]);
        $arg1->addAttribute("type", $info[0]);
    }

    return CODE_OK;
}


#save statistics to file
function save_stats($stats, $loc_first, $loc, $comments, $loc_number, $comments_number, $file) {
    if ($stats === true) {
        $file_obj = fopen($file, "w") or exit(FILE_ERROR);
        if ($loc_first === true) {
            if ($loc === true) {
                fwrite($file_obj, $loc_number);
                fwrite($file_obj, "\n");
            }
            if ($comments === true) {
                fwrite($file_obj, $comments_number);
                fwrite($file_obj, "\n");
            }
        } else {
            if ($comments === true) {
                fwrite($file_obj, $comments_number);
                fwrite($file_obj, "\n");
            }
            if ($loc === true) {
                fwrite($file_obj, $loc_number);
                fwrite($file_obj, "\n");
            }
        }
        fclose($file_obj);
    }
}


# Funkcia parsuje vstup (.ippcode18) a vypise vystup XML
function start_parsing(&$comment_lines, &$instr_line) {

    #$xml = new SimpleXMLElement('<program/>'); #output xml
    $xml = new SimpleXMLElement('<?xml version="1.0" encoding="UTF-8"?><program/>');
    $xml->addAttribute("language", "IPPcode18");

    #initializing states
    $current_state = "init";
    $order = 1;

    while ($line = fgets(STDIN)) { #nacitanie instrukcie

        #odstranenie komentarov
        $raw_line = $line;
        $comment_start = strpos($line, '#');
        if ($comment_start !== false) {
            $raw_line = substr($line, 0, $comment_start);
            $comment_lines += 1;
        }
        if (($raw_line == "" or ctype_space($raw_line)) and ($current_state == "load_inst")) {
            continue;
        }

        switch ($current_state) { #automat

            case "init":
                #$command = substr(strtolower($raw_line), 0, -1);
                $command = rtrim(strtolower($raw_line));
                if ($command == ".ippcode18") { #uvodny retazec
                    $current_state = "load_inst";
                }
                else {
                    fwrite(STDERR, "Code doesn't start with .IPPcode18 [case insensitive]\n");
                    exit(LS_ERROR);
                }
                break;

            case "load_inst":
                $code = recognize_inst($raw_line, $order, $xml);
                if ($code == CODE_OK) {
                    $instr_line += 1;
                    $order = $order + 1;
                }
                else {
                    fwrite(STDERR, "Lexical or syntactic error, check your code.\n");
                    exit(LS_ERROR);
                }
                break;
        }
    }
    if ($order == 1 and $current_state==="init") {
        exit(LS_ERROR);
    }
    #vypisanie vytvoreneho XML formatu
    $dom = dom_import_simplexml($xml)->ownerDocument;
    $dom->formatOutput = true;
    echo $dom->saveXML();
}


#Hlavny program
error_reporting(0);
$file = "";
$stats = false;
$loc_first = false;
$loc = false;
$comments = false;
$comment_lines = 0;
$instruction_lines = 0;
check_params($argv, $stats, $loc_first, $loc, $comments, $file);
start_parsing($comment_lines, $instruction_lines);
save_stats($stats, $loc_first, $loc, $comments, $instruction_lines, $comment_lines, $file);