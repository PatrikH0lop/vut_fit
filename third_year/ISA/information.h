#ifndef INFORMATION_H
#define INFORMATION_H

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::vector;


class GeneralInformation {

public:
    string title;
    string url;

    GeneralInformation();
};


class AtomEntryInformation: public GeneralInformation {

public:
    string published;
    string last_updated;
    string alternate_url;
    vector <string> authors;
    vector <string> source_authors;

    // extensions
    vector <string> dc_creators;
    string dc_date;

    AtomEntryInformation();
};


class AtomEntryInformationSummary {

public:
    vector <string> global_authors;
    vector <AtomEntryInformation> entry_information;

    void print_info(bool sep_newline);
};


class RssItemInformation: public GeneralInformation {

public:
    string author;
    string last_updated;
    string guid_permalink;

    // extensions
    string dc_creator;
    string dc_date;

    RssItemInformation();
};


class RssItemInformationSummary {

public:
    vector <RssItemInformation> item_information;

    // extensions
    string dc_creator;

    void print_info(bool sep_newline);
};


#endif // INFORMATION_H
