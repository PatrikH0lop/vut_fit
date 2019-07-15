#include "file.h"


vector <string> get_source_list(string filename) {
    
    ifstream input_file(filename);
    string line; 
    vector <string> sources;

    if (input_file.is_open()) {
        while (getline(input_file, line)) {
            // ignore empty and comment lines
            if (line == "" || line.find("#") == 0)
                continue;
            sources.push_back(line);
        }
    } else {
        raise_error(FILE_OPEN_ERROR);
    }

    return sources;
}