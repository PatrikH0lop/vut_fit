#ifndef RSS_READER_CLIENT_H
#define RSS_READER_CLIENT_H

#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <ctype.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <libxml/tree.h>
#include <libxml/parser.h>

#include "information.h"
#include "errors.h"

using std::cout;
using std::vector;
using std::string;
using std::regex;


class ConnectionClient {

private:
    int port = 80;
    int buffer_size = 1024;

    bool author_flag = false;
    bool url_flag = false;
    bool updated_flag = false;

    void parse_rss_v1_content(xmlNode *root_element);
    void parse_rss_v2_content(xmlNode *root_element);
    RssItemInformationSummary parse_all_rss_items(xmlNode *root_element, bool inside_channel);
    RssItemInformation parse_rss_item(xmlNode *item);

    void parse_atom_content(xmlNode *root_element);
    AtomEntryInformationSummary parse_all_atom_entries(xmlNode *root_element);
    AtomEntryInformation parse_atom_entry(xmlNode *item);

public:
    void initialize_ssl();
    void set_flags(bool auth_flag, bool url_flag, bool updated_flag);
    string get_file(string url, string certfile, string certaddr);
    void parse_xml_content(string content);
    bool has_flag();

};


// Functions
string get_feed_type_information(xmlNode *root_node);
string get_feed_title_information(xmlNode *root_node, string feed_type);
string look_for_property_value(string property_name, xmlNode *item);
string look_for_attribute(string property_name, xmlNode *item);
string look_for_atom_link(xmlNode *item, string testing_criterion);
xmlNode *look_for_property(string property_name, xmlNode *item);
vector <string> look_for_all_atom_authors(xmlNode *item);
vector <string> look_for_all_source_authors(xmlNode *item);
vector <string> look_for_all_atom_dc_authors(xmlNode *item);
bool is_url(string url);
#endif //RSS_READER_CLIENT_H
