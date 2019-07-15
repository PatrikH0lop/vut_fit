#include "errors.h"


void print_help() {
    cout << "Help:" << endl
    << "./feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]" << endl
    << "Feedfile: File, that contains URLs of feed sources" << endl
    << "certfile: Certificate file" << endl
    << "certaddr: Folder containing certificate files" << endl
    << "-T:    Prints last time of change" << endl
    << "-a:    Prints author" << endl
    << "-u:    Prints associated URL" << endl
    << "-h:    Prints help information (other parameters are ignored)" << endl;
}


void raise_error(int error_code) {
    switch (error_code) {
        case WRONG_PARAMS:
            cerr << "Error: Wrong parameter" << endl;
            break;
        case DUPLICIT_PARAM:
            cerr << "Error: Duplicit parameter" << endl;
            break;
        case INVALID_PARAM_VALUE:
            cerr << "Error: Invalid value of the parameter" << endl;
            break;
        case URL_PARAM_VALUE:
            cerr << "Error: URL parameter value" << endl;
            break;
        case RUNTIME_ERROR:
            cerr << "Error: Runtime error" << endl;
            break;
        case CONNECTION_ERROR:
            cerr << "Error: Connection failure" << endl;
            break;
        case HTTPS_CONNECTION_ERROR:
            cerr << "Error: Https connection failure" << endl;
            break;
        case CERTIFICATE_VERIFICATION_ERROR:
            cerr << "Error: Certification verification error" << endl;
            break;
        case XML_HTTP_CONTENT_ERROR:
            cerr << "Error: HTML content parse error" << endl;
            break;
        case XML_PARSE_ERROR:
            cerr << "Error: XML parse error" << endl;
            break;
        case XML_HEADER_ERROR:
            cerr << "Error: XML Header parsing (file type info)" << endl;
            break;
        case XML_CONTENT_ERROR:
            cerr << "Error: XML Content parsing error (file type content)" << endl;
            break;
        case FEED_TITLE_ERROR:
            cerr << "Error: Feed title extraction" << endl;
            break;
        case URL_PARSE_ERROR:
            cerr << "Error: Parsing URL" << endl;
            break;
        case NOT_IMPLEMENTED_ERROR:
            cerr << "Error: Not implemented yet" << endl;
            break;
        case FILE_OPEN_ERROR:
            cerr << "Error: File open error" << endl;
            break;
        case CERT_VERIFICATION_ADDR_ERROR:
            cerr << "Error: Certificate verifying (addr)" << endl;
            break;
        case CERT_VERIFICATION_FILE_ERROR:
            cerr << "Error: Certificate verifying (file)" << endl;
            break;
    }

    throw std::runtime_error("Error");
}
