#ifndef FEEDREADER_H
#define FEEDREADER_H

#include <iostream>
#include <string>
#include <unistd.h>
#include <exception>

#include "errors.h"
#include "client.h"
#include "file.h"

using std::cout;
using std::cerr;

// Functions

int parse_params(int argc, char *argv[]);


#endif // FEEDREADER_H 