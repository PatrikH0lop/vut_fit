/*
 * File:    proj1.c
 * Date:    October 2016
 * Author:  Patrik Holop, xholop01@stud.fit.vutbr.cz
 * Project: Text processing
 * About:   Program is used to transfer binary date on input to formatted text data
 *          and reverse. Input can be modified with different arguments
 */

// Header files
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

// Defined values and constants
#define UNLIMITED_CHARS -1
#define MAX_LENGTH 200

// Errors
#define ARG_ERROR 1
#define INPUT_ERROR 2

// Tutorial
const char *HELP = 
    "Author:  Patrik Holop\n"
    "Program: Project 1\n\n"
    "How to use: proj1 [-s] N [-n] M\n"
    "            proj1 -r           \n"
    "            proj1 -x           \n"
    "            proj1 -S N\n"
    "Description:\n"
    "[-s] N : skip N characters on input, N>=0\n"
    "[-n] M : read max M characters on input, M>0\n" 
    "    -r : transfers binary data to text\n"
    "    -x : transfers text data to binary\n"
    "  -S N : prints substrings of printable or blank\n"
    "         chars longer than N, N>0\n" 
;

// List of functions
int  length(char *s);
bool compare_strings(char *s1, char *s2);
bool is_hex_value(char character);
bool is_middle(int position);
int  process_arguments();
void print_hex(int character, int position, bool fill);
void print_buffer(int s[], bool special);
void skip_input(int skip);
void text_to_binary(int skip, int max_to_read);
void text_to_binary_unformatted();
int  binary_to_text();
void print_substrings(int minimal_length);

/**
 * Returns length of string
 * @param s string
 * @return Returns length of string
*/
int length(char *s) {
    int len = 0;
    while(s[len]!='\0') len++;
    return len;
}

/**
 * Compares 2 strings
 * @param s1 First string
 * @param s2 Second string
 * @return Returns true if strings are equal, else false
*/
bool compare_strings(char *s1, char *s2) {
    int lengthS1 = length(s1);
    int lengthS2 = length(s2);
    if (lengthS1!=lengthS2)
        return false;
    else {
        for (int i=0; i<lengthS1; i++) {
            if (s1[i]!=s2[i])
                return false;
        }
    }
    return true;
}

/**
 * Prints string
 * @param array   string to print
 * @param special if true, prints buffer in format |bbbbbbbbbbbbbbbb|
*/
void print_buffer(int array[], bool special) {
    if (special) {
        printf("  |");
        for (int i=0; i<16; i++) {
            if (isprint(array[i]))
                printf("%c",array[i]);
            else printf("%c",'.');
        }
        printf("|");
    }
    else {
        int position = 0;
        while (array[position]!=EOF) {
            printf("%c",array[position]);
            position++;
        }
    }
    printf("\n");
}

/**
 * Determines if position of char in string of length 16 chars is middle
 * @param position Position of character
 * @return true, if it is middle position, else false
 */
bool is_middle(int position) {
    if (position%8==0 && position%16!=0)
            return true;
    return false;
}

/**
 * Prints hexadecimal value of character in format 'xx'
 * @param character Its ASCII code is value to print
 * @param position If middle position, prints longer distance between hex values
 * @param fill If true, just filling |bbbbbbbbbbbbbbbb| buffer with ' ' characters
 */
void print_hex(int character, int position, bool fill) {
    if (is_middle(position))
        if (fill)
            printf("    ");
        else
            printf("  %02x", character);
    else
        if (fill)
            printf("   ");
        else
            printf(" %02x", character);
}

/**
 * Skips first characters on input (-s number)
 * @param skip Number of characters to skip
 */
void skip_input(int skip) {
    int counter = 0;
    int character;
    while ((counter<skip) && ((character=getchar())!=EOF)) {
        counter++;
    }
}

/**
 * Transfers text data to binary in format
 * aaaaaaaa xx xx xx xx xx xx xx xx  xx xx xx xx xx xx xx xx  |bbbbbbbbbbbbbbbb|
 * aaaaaaaa - address of first character
 * xx - hex value of character
 * b - character
 * @param skip        Number of characters to skip on input
 * @param max_to_read Maximal number of characters to read on input
 */
void text_to_binary(int skip, int max_to_read) {

    int real_position = skip;
    int character_counter = 0;
    if (skip!=0)                                
            skip_input(skip);
    if (max_to_read == 0)                              
            character_counter = UNLIMITED_CHARS;
    int character;
    int buffer[17];
    int position = 0;

    while ((character_counter < max_to_read) && (character=getchar())!=EOF) {
        position = real_position-skip;
        if (position %16 == 0)  // if 1st character of line, print its position
            printf("%08x ",real_position);
        print_hex(character, position, false); // print hex data
        buffer[position % 16]=character;   // store char in buffer
        if (position %16 == 15) {  // if last character of line, print buffer
            print_buffer(buffer,true);
        }

        real_position++; // move to next character
        if (character_counter != UNLIMITED_CHARS)  // if maximum number of chars was not set, increase length
            character_counter++;
    }

    position = real_position-skip;
    if (position%16<=15 && position%16!=0) {     // if buffer is not full, fulfill it with ' '
        while (position%16!=0) {
            buffer[(real_position-skip)%16]=' ';
            print_hex(' ', position, true);
            real_position++;
            position = real_position-skip;
            }
        print_buffer(buffer,true);
    }

    return;
}

/**
 * Transfers text to binary data
*/
void text_to_binary_unformatted() {
    int character;
    while ((character=getchar())!=EOF) {
        printf("%02x",character);
    }
    printf("%c",'\n');
}

/**
 * Prints substrings of printable or blank characters of mininal length
 * @param minimal_length Minimal length of string to print
 */
void print_substrings(int minimal_length) {

    int substring[MAX_LENGTH];
    int position = 0;
    int character;

    while ((character=getchar())!=EOF) {
        if ((isprint(character) || isblank(character)) && position<MAX_LENGTH) {
            substring[position]=character;
            position++;
        }
        else {
            substring[position]=EOF;
            if (position>=minimal_length)
                print_buffer(substring, false);
            position = 0;
        }
    }
	
    // print last string shorter than 200 chars
    if (position!=0) {
        substring[position]=EOF;
        if (position>=minimal_length)
                print_buffer(substring, false);
    }

}

/**
 * Determines if character is hexadecimal value
 * @param character
 * @return true if is hexadecimal value, else returns false
 */
bool is_hex_value(char character) {
     if (((character>='a' && character<='f') || (character>='A' && character<='F')) || isdigit(character))
        return true;
     else
        return false;
}

/**
 * Transfers binary data to text (-r)
 * @return return 0 if completed successfully, else return INPUT_ERROR
 */
int binary_to_text() {

    int position = 0;
    char character;
    char hex_value[3];
    hex_value[2]='\0';

    while ((character=getchar())!=EOF) {
        if (is_hex_value(character)) {
            hex_value[position]=character;
            position++;
            // if hexadecimal digit consists of 2 chars, print it
            if (position==2) { 
                position=0;
                long converted_number = strtol(hex_value, NULL, 16);
                int printable_char = (int)converted_number;
                printf("%c",printable_char);
            }
        }
	else {
	    // error, if char is neither hex value or white 
	    if (!isspace(character)) {
		return INPUT_ERROR;
	    }
        } 
    }

    // if 1 digit remains to transfer
    if (position==1) { 
        hex_value[1]='\0';
        long converted_number = strtol(hex_value, NULL, 16);
        int printable_char = (int)converted_number;
        printf("%c",printable_char);
    }

    return 0;
}

/**
 * Runs the correct function based on arguments
 * @param argc Number of arguments
 * @param argv Arguments on input
 * @return 0 if arguments on input are correct, else returns ARG_ERROR
 */

int process_arguments(int argc, char *argv[]) {
    // no arguments 
    if (argc==1)
        text_to_binary(0,0);

    // 1 argument, -x or -r
    else if (argc == 2) {
        if (compare_strings(argv[1],"-x"))
            text_to_binary_unformatted();
        else if (compare_strings(argv[1],"-r")) {
            	if(binary_to_text()==INPUT_ERROR)
		    return INPUT_ERROR;
	     }
        else 
            return ARG_ERROR;
    }

    // 2 arguments, -s N or -n M or -S N
    else if(argc == 3) {
        char *help;
        long number = strtol(argv[2],&help,10);
        if (*help!='\0') {
            return ARG_ERROR;
            }
        else if (compare_strings(argv[1],"-s") && (number>=0))
            text_to_binary(number,0);
        else if (compare_strings(argv[1],"-n") && (number>0))
            text_to_binary(0,number);
        else if (compare_strings(argv[1], "-S") && (number>0))
            print_substrings(number);
        else 
	    return ARG_ERROR;
    }
    
    // 4 arguments, -s N -n M or -n M -s N
    else if (argc == 5) {
        char *help1,*help2;
        long number1 = strtol(argv[2],&help1,10);
        long number2 = strtol(argv[4],&help2,10);
        if (*help1!='\0' || *help2!='\0') 
            return ARG_ERROR;
        else if (compare_strings(argv[1],"-s") && compare_strings(argv[3],"-n") && (number1>=0) && (number2>0))
            text_to_binary(number1,number2);
        else if (compare_strings(argv[1],"-n") && compare_strings(argv[3],"-s") && (number2>=0) && (number1>0))
            text_to_binary(number2,number1);
        else 
            return ARG_ERROR;
    }

    // more or different arguments
    else 
        return ARG_ERROR; 

    return 0;
}


/**
 * Main body
 * @param argc Number of arguments
 * @param argv Arguments
 * @return 0 if program ends successfully, else return -1  
 */
int main(int argc, char *argv[])
{
    int process = process_arguments(argc,argv);
    // wrongs arguments
    if(process==ARG_ERROR) {
	printf("%s",HELP);
	return 0;
    }
    // wrong input data
    else if (process==INPUT_ERROR) {
	fprintf(stderr, "Wrong input data\n");
	return -1;
    };
    return 0;
}
