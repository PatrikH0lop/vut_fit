<?php
#Autor: xholop01
#IPP1 : Testovaci subor


#Programove konstanty
define("NAME_ERROR", -1);
define("WRONG_PARAM", 10);
define("LS_ERROR", 21);
define("CODE_OK", 0);
define("CODE_FAIL", 1);
define("FILE_ERROR", 12);
define("INPUT_FILE_ERROR", 11);
define("NOT_FOUND", 1);
define("FOUND", 0);


#Vypis varovania o zlych argumentoch
function print_warning() {
    fwrite(STDERR, "Bad arguments, run script with --help\n");
}


#Kontrola parametrov
function check_params($argv, &$directory, &$path, &$recursive, &$parse, &$interpret)
{
    if (count($argv) > 1) {
        $help = false;
        $directory = false;
        $parse_script = false;
        $int_script = false;
        $recursive = false;
        $parse = "parse.php";
        $interpret = "interpret.py";
        $path = "./";

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
                case "--recursive":
                    if ($recursive == false) {
                        $recursive = true;
                    } else {
                        print_warning();
                        exit(WRONG_PARAM);
                    }
                    break;
                default:
                    if (strpos($arg, "--directory=") === 0) {
                        if ($directory == false) {
                            $path = substr($arg, 12);
                            if (substr($path, -1) !== "/") {
                                $path = $path . "/";
                            }
                            if (file_exists($path) === false) {
                                fwrite(STDERR, "Nonexisting test directory!\n");
                                exit(INPUT_FILE_ERROR);
                            }
                            #echo $path;
                            $directory = true;
                        } else {
                            print_warning();
                            exit(WRONG_PARAM);
                        }
                    } else if (strpos($arg, "--parse-script=") === 0) {
                        if ($parse_script == false) {
                            $parse = substr($arg, 15);
                            if (file_exists($parse) === false) {
                                fwrite(STDERR, "Nonexisting parse script!\n");
                                exit(INPUT_FILE_ERROR);
                            }
                            $parse_script = true;
                        } else {
                            print_warning();
                            exit(WRONG_PARAM);
                        }
                    } else if (strpos($arg, "--int-script=") === 0) {
                        if ($int_script == false) {
                            $interpret = substr($arg, 13);
                            if (file_exists($interpret) === false) {
                                fwrite(STDERR, "Nonexisting interpret script!\n");
                                exit(INPUT_FILE_ERROR);
                            }
                            $int_script = true;
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
        if ($help === true and $directory === false and $recursive === false and $parse_script === false and $int_script === false) {
            echo "Usage: php5.6 test.php [--help] [--directory=test_path] [--recursive]\n\t\t       [--parse-script=file] [--int-script=file]\n";
            exit(CODE_OK);
        } else if ($help === true) {
            print_warning();
            exit(WRONG_PARAM);
        }
    }
}


#generuje chybajuce subory v pripade testov
function generate_missing_files($path, $filename, $record) {
    if ($record[$filename.".in"] == NOT_FOUND) {
        $file = fopen($path.$filename.".in", "w");
        if (! $file) {
            fwrite(STDERR, "Could not generate .in file\n");
            return FILE_ERROR;
        }
        fclose($file);
    }
    if ($record[$filename.".out"] == NOT_FOUND) {
        $file = fopen($path.$filename.".out", "w");
        if (! $file) {
            fwrite(STDERR, "Could not generate .out file\n");
            return FILE_ERROR;
        }
        fclose($file);
    }
    if ($record[$filename.".rc"] == NOT_FOUND) {
        $file = fopen($path.$filename.".rc", "w");
        if (! $file) {
            fwrite(STDERR, "Could not generate .rc file\n");
            return FILE_ERROR;
        }
        fwrite($file, "0");
        fclose($file);
    }
}


#vyhlada vsetky testy v zadanej adresarovej strukture
function find_tests($directory, $path, $recursive, $parse, $interpret, &$found_files, &$counter) {

    $files = scandir($path);

    foreach ($files as $file) {
        if (!is_dir($path.$file)) {
            if (strpos($file, ".src") == strlen($file)-4 and strpos($file, ".src") !== false) {
                $filename = substr($file, 0, -4);
                #komplenty zaznam o testoch aj pre generovanie a testovanie
                $record =  array("id" => $counter, "path" => $path, "filename" => $filename, $filename.".in" => NOT_FOUND,
                                 $filename.".out" => NOT_FOUND, $filename.".rc" => NOT_FOUND, "php_result" => CODE_FAIL,
                                 "py_result" => CODE_FAIL, "expected_rc" => CODE_FAIL, "diff_res" => CODE_FAIL, "fin_res" => CODE_FAIL);
                foreach ($files as $c_file) {
                    if (!is_dir($path.$c_file)) {
                        if ($c_file == $filename.".in") {
                            $record[$filename.".in"] = FOUND;
                        }
                        if ($c_file == $filename.".out") {
                            $record[$filename.".out"] = FOUND;
                        }
                        if ($c_file == $filename.".rc") {
                            $record[$filename.".rc"] = FOUND;
                        }
                    }
                }
                generate_missing_files($path, $filename, $record);
                array_push($found_files, $record);
                $counter += 1;
            }
        }
        else {
            $full_path = $path.$file;
            #echo "DIR: " . $full_path  . "\n";
            if ($recursive === true and $file != "." and $file != "..") {
                find_tests($directory, $full_path . "/", $recursive, $parse, $interpret, $found_files, $counter);
            }
        }
    }

    return CODE_OK;
}


#vykonanie vsetkych najdenych testov
function run_tests(&$tests, $parse, $interpret)
{
    foreach ($tests as $test) {
        $return_var = 1;
        $output = "";
        $id = $test["id"];
        exec("cat " . $test["path"] . $test["filename"] . ".src |" . " php5.6 " . $parse . " > t_output.temp 2> test_rer.err", $output, $return_var);
        $tests[$id]["php_result"] = $return_var;
        $exp_file = trim(file_get_contents($test["path"] . $test["filename"] . ".rc"));
        $tests[$id]["expected_rc"] = intval($exp_file);
        if ($return_var == 0) {
            exec("python3.6 " . $interpret . " --source=t_output.temp < " . $test["path"] . $test["filename"] . ".in" . " > tpy_output.temp 2> test_rer.err", $output, $return_var);
            $tests[$id]["py_result"] = $return_var;
            exec("diff tpy_output.temp " . $test["path"] . $test["filename"] . ".out", $output, $return_var);
            $tests[$id]["diff_res"] = $return_var;
            if ($tests[$id]["py_result"] == $tests[$id]["expected_rc"] and $tests[$id]["py_result"] != CODE_OK) {
                $tests[$id]["fin_res"] = CODE_OK;
            } else if ($tests[$id]["py_result"] == CODE_OK and $tests[$id]["diff_res"] == CODE_OK and $tests[$id]["expected_rc"] == CODE_OK) {
                $tests[$id]["fin_res"] = CODE_OK;
            }
        } else {
            if ($tests[$id]["php_result"] == $tests[$id]["expected_rc"]) {
                $tests[$id]["fin_res"] = CODE_OK;
            }
        }
    }
    unlink("t_output.temp");
    unlink("tpy_output.temp");
    unlink("test_rer.err");
    return CODE_OK;
}


#generajuce vysledne html
function generate_html($record) {
    $xml = new SimpleXMLElement('<!DOCTYPE html><html/>');
    $xml->addAttribute("lang", "en");
    $head = $xml->addChild("head");
    $head->addChild("title", "IPP Projekt: Test results");
    $head->addChild("style", "table, th, td { border: 1px solid black; }");
    $body = $xml->addChild("body");
    $body->addChild("h");
    $title = $body->addChild("b");
    $title = $title->addChild("font", "Projekt IPP: Test results");
    $title->addAttribute("size", "4");
    $table = $body->addChild("table");
    $table->addAttribute("width:100%");
    $table->addChild("p");
    $headertable = $table->addChild("tr");
    $headertable->addChild("th", "Number of tests");
    $headertable->addChild("th", "PASSED");
    $headertable->addChild("th", "FAILED");
    $num = 0;
    $pass = 0;
    $fail = 0;
    foreach ($record as $test) {
        if ($test["fin_res"] == CODE_OK) $pass++;
        if ($test["fin_res"] == CODE_FAIL) $fail++;
        $num++;
    }
    $child = $table->addChild("tr");
    $child->addChild("td", $num);
    $child->addChild("td", $pass);
    $child->addChild("td", $fail);
    $body->addChild("p");

    $table = $body->addChild("table");
    $table->addAttribute("width:100%");;
    $headertable = $table->addChild("tr");
    $headertable->addChild("th", "Test file");
    $headertable->addChild("th", "Parse rc");
    $headertable->addChild("th", "Interpret rc");
    $headertable->addChild("th", "DIFF rc");
    $headertable->addChild("th", "Expected rc");
    $headertable->addChild("th", "RESULT");

    foreach ($record as $test) {
        $child = $table->addChild("tr");
        $child->addChild("td", $test["path"].$test["filename"].".src");
        $child->addChild("td", $test["php_result"]);
        $child->addChild("td", $test["py_result"]);
        $child->addChild("td", $test["diff_res"]);
        $child->addChild("td", $test["expected_rc"]);
        if ($test["fin_res"] == CODE_OK) {
            $td = $child->addChild("td");
            $res = $td->addChild("font", "PASS");
            $res->addAttribute("color", "green");
        }
        else {
            $td = $child->addChild("td");
            $res = $td->addChild("font", "FAIL");
            $res->addAttribute("color", "red");
        }
    }
    #vypisanie vytvoreneho XML formatu
    $dom = dom_import_simplexml($xml)->ownerDocument;
    $dom->formatOutput = true;
    echo $dom->saveHTML();
}


#hlavny program
$directory = false;
$path = "./";
$recursive = false;
$parse = "parse.php";
$interpret = "interpret.py";
$counter = 0;
error_reporting(0);
check_params($argv,$directory,$path, $recursive, $parse, $interpret);

$found_files = array();
find_tests($directory, $path, $recursive, $parse, $interpret, $found_files, $counter);
run_tests($found_files, $parse, $interpret);
generate_html($found_files);