#ifndef ISA_ERRORS_H
#define ISA_ERRORS_H


#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>

using std::cout;
using std::cerr;
using std::endl;


// Errors definitions
#define OK 0

// Parameter

#define WRONG_PARAMS 1
#define DUPLICIT_PARAM 2
#define INVALID_PARAM_VALUE 3
#define URL_PARAM_VALUE 4

// Connection
#define CONNECTION_ERROR 5
#define XML_HTTP_CONTENT_ERROR 6
#define HTTPS_CONNECTION_ERROR 7
#define CERTIFICATE_VERIFICATION_ERROR 8

// XML Errors
#define XML_PARSE_ERROR 10
#define XML_HEADER_ERROR 11
#define XML_CONTENT_ERROR 12

// Feed Errors
#define FEED_TITLE_ERROR 20
#define URL_PARSE_ERROR 21

// File Errors
#define FILE_OPEN_ERROR 30

// Certification Error
#define CERT_VERIFICATION_ADDR_ERROR 40
#define CERT_VERIFICATION_FILE_ERROR 41

// Runtime
#define NOT_IMPLEMENTED_ERROR 98
#define RUNTIME_ERROR 99


// Functions
void raise_error(int error_code);
void print_help();

#endif //ISA_ERRORS_H
