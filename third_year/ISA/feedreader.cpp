#include "feedreader.h"


void assign_file_param(string *param, char *value) {

    if (*param != "") {
        print_help();
        raise_error(DUPLICIT_PARAM);
    }
    else if (value) {
        *param = optarg;
    } else {
        print_help();
        raise_error(INVALID_PARAM_VALUE);
    }
}


int parse_params(int argc, char *argv[]) {

    // params definition
    string url = "";
    string feedfile = "";
    string certfile = "";
    string certaddr = "";
    bool time_flag = false;
    bool author_flag = false;
    bool url_flag = false;
    bool help_flag = false;
    bool parse_test_flag = false;

    // iterating variable
    int c = 0;

    // parsing params in the loop
    while ((c=getopt(argc, argv, "f:c:C:Tauhp")) != -1) {
        switch (c) {
            // feedfile parameter
            case 'f':
                assign_file_param(&feedfile, optarg);
                break;
            // certfile
            case 'c':
                assign_file_param(&certfile, optarg);
                break;
            // certaddr
            case 'C':
                assign_file_param(&certaddr, optarg);
                break;
            // time flag
            case 'T':
                time_flag = true;
                break;
            // author flag
            case 'a':
                author_flag = true;
                break;
            // url flag
            case 'u':
                url_flag = true;
                break;
            case 'h':
                help_flag = true;
                break;
            // parser test flag
            case 'p':
                parse_test_flag = true;
                break;
            // unknown parameter
            default:
                print_help();
                raise_error(WRONG_PARAMS);
        }
    }

    // print help
    if (help_flag) {
        print_help();
        exit(0);
    }

    if (!parse_test_flag) {
        // check if there is more non-optional arguments
        if (argc-optind > 1) {
            print_help();
            raise_error(WRONG_PARAMS);
        } else {
            // check if are not set feedfile and url both
            if (argc-optind == 1 && feedfile != "") {
                print_help();
                raise_error(WRONG_PARAMS);
            // check if either one of those were set
            } else if (argc-optind == 0 && feedfile == "") {
                print_help();
                raise_error(WRONG_PARAMS);
            // set url
            } else if (argc-optind == 1) {
                url = argv[optind];
            }
        }
    }

    ConnectionClient client;
    client.set_flags(author_flag, url_flag, time_flag);
    client.initialize_ssl();

    // parse one url
    if (parse_test_flag) {
        string input_xml = "";
        try {
            for (std::string line; std::getline(std::cin, line);) {
                input_xml += line;
            }
            client.parse_xml_content(input_xml);
        } catch (std::exception &e) {
            exit(1);
        }
    }
    else if (url != "") {
        try {
            string xml_content = client.get_file(url, certfile, certaddr);
            client.parse_xml_content(xml_content);
        } catch (std::exception& e) {
            exit(1);
        }
    // parse urls from source file
    } else {
        vector <string> sources = get_source_list(feedfile);
        for (int i = 0; i < sources.size(); i++) {
            try {
                string xml_content = client.get_file(sources[i], certfile, certaddr);
                client.parse_xml_content(xml_content);
                if (i != sources.size()-1)
                    cout << endl;
            } catch (std::exception &e) {
                cout << endl;
                continue;
            }
        }
    }
}


int main(int argc, char *argv[]) {

    // parse arguments
    try {
        parse_params(argc, argv);
    } catch (std::exception &e) {
        exit(1);
    }

    return 0;
}