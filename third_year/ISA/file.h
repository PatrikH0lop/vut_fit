#ifndef RSS_READER_FILE_H
#define RSS_READER_FILE_H

#include <vector>
#include <string>
#include <fstream>

#include "errors.h"

using std::vector;
using std::string;
using std::ifstream;
using std::getline;

// Functions 

vector <string> get_source_list(string filename);

#endif // RSS_READER_FILE_H
