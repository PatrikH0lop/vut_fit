#include "client.h"

// Connection Client functions


void ConnectionClient::initialize_ssl() {
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
}


void ConnectionClient::set_flags(bool auth_flag, bool url_flag, bool updated_flag) {
    this->author_flag = auth_flag;
    this->url_flag = url_flag;
    this->updated_flag = updated_flag;
}


bool ConnectionClient::has_flag() {
    if (this->author_flag || this->url_flag || this->updated_flag)
        return true;
    return false;
}


string ConnectionClient::get_file(string url, string certfile, string certaddr) {

    regex http_regex("(http|https)://(.*?)(/.*)");
    regex host_regex("(.*):(.*)");
    std::cmatch cm;
    bool custom_port = false;

    std::regex_match(url.c_str(), cm, http_regex);
    if (cm.size() != 4) {
        raise_error(URL_PARAM_VALUE);
    }

    string prot = cm[1];
    string host = cm[2];
    string suffix = cm[3];
    string port = "80";

    std::regex_match(host.c_str(), cm, host_regex);
    
    if (cm.size() == 3) {
        port = cm[2];
        host = cm[1];
        custom_port = true;
    }

    // socket pointer
    BIO *bio;

    // setup unsecure connection
    if (prot == "http") {
        
        // connect to socket
        bio = BIO_new_connect((host + ":" + port).c_str());

        // connection setup failure
        if (bio == NULL) {
            raise_error(CONNECTION_ERROR);
        }

        // connect
        if (BIO_do_connect(bio) <= 0) {
            raise_error(CONNECTION_ERROR);
        }
    }
    // setup secure connection
    else if (prot == "https") {

        if (!custom_port)
            port = "443";

        // secure structures
        SSL *ssl;
        SSL_CTX *ctx;

        // ssl context
        ctx = SSL_CTX_new(SSLv23_client_method());

        // use default location if no cert specification
        if (certfile == "" && certaddr == "")
            SSL_CTX_set_default_verify_paths(ctx);
        else {
            // used certfile or certaddr
            if (certfile != "") {
                if (! SSL_CTX_load_verify_locations(ctx, certfile.c_str(), NULL)) {
                    raise_error(CERT_VERIFICATION_FILE_ERROR);
                }
            } 
            if (certaddr != "") {
                if (! SSL_CTX_load_verify_locations(ctx, NULL, certaddr.c_str())) {
                    raise_error(CERT_VERIFICATION_ADDR_ERROR);
                }
            }
        }

        // set connection
        bio = BIO_new_ssl_connect(ctx);

        // ssl retry flag
        BIO_get_ssl(bio, &ssl);
        SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

        // connect
        BIO_set_conn_hostname(bio, (host + ":" + port).c_str());

        // check connection
        if (BIO_do_connect(bio) <= 0) {
            BIO_free_all(bio);
            SSL_CTX_free(ctx);
            raise_error(HTTPS_CONNECTION_ERROR);
        }

        // check certificate 
        if (SSL_get_verify_result(ssl) != X509_V_OK) {
            BIO_free_all(bio);
            SSL_CTX_free(ctx);
            raise_error(CERTIFICATE_VERIFICATION_ERROR);
        }
    }
    
    // make GET request
    string request = "GET " + suffix + " HTTP/1.1\r\n"
    "Host: " + host + "\r\n"
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.100 Safari/537.36\r\n"
    "Connection: Close\r\n\r\n";    

    BIO_write(bio, request.c_str(), request.length());

    // read from the connection
    char buffer[this->buffer_size];
    string http_message = "";

    while (true) {
        // read another message
        int response = BIO_read(bio, buffer, this->buffer_size-1);
        
        // message ended
        if (response <= 0) {
            break;
        }

        // end the string
        buffer[response] = 0;
        string buf(buffer, response);
        http_message += buf;
        
        // clean buffer
        for (int i = 0; i < this->buffer_size; i++)
            buffer[i] = 0;
    }
    
    string complete_xml_message = "";
    string http_header = "";
    size_t xml_start;
    // list of possible prefixes, order is important
    vector <string> possible_prefixes = {"<?xml", "<rdf:RDF", "<feed", "<rss"};

    // find XML content boundaries
    bool prefix_found = false;

    for (string xml_prefix: possible_prefixes) {
        xml_start = http_message.find(xml_prefix);
        if (xml_start != std::string::npos) {
            prefix_found = true;
            break;
        }
    }

    // continue to parse
    if (prefix_found) {
        http_header = http_message.substr(0, xml_start);
    } else {        
        raise_error(XML_HTTP_CONTENT_ERROR);
    }

    // get XML content
    complete_xml_message = http_message.substr(xml_start, http_message.size()-xml_start);

    // get init chunk size if chunked
    string chunk_size_str = "";
    int init_offset = 3;
    char c = http_header[http_header.size()-init_offset];    

    // http is chunked
    if (isxdigit(c)) {
        // get init chunk size
        while (isxdigit(c)) {
            chunk_size_str = c + chunk_size_str;
            init_offset += 1;
            c = http_header[http_header.size()-init_offset];
        }
        int chunk_size = stoi(chunk_size_str, NULL, 16);
        int last_chunk_size = chunk_size;

        // remove all chunk size information thourh iteration
        while (chunk_size != 0) {
            init_offset = 2;
            chunk_size_str = "";
            c = complete_xml_message[last_chunk_size + init_offset];            
            while (isxdigit(c)) {
                chunk_size_str += c;
                init_offset += 1;
                c = complete_xml_message[last_chunk_size + init_offset];
            }                        
            chunk_size = stoi(chunk_size_str, NULL, 16);            
            int to_remove = 2 + chunk_size_str.size() + 2;                        
            complete_xml_message.erase(last_chunk_size, to_remove);
            last_chunk_size += chunk_size;
        }
    }
    
    BIO_free_all(bio);
    return complete_xml_message;
}

void foo(...) {}

void ConnectionClient::parse_xml_content(string content) {

    xmlGenericErrorFunc handler = (xmlGenericErrorFunc)foo;
    initGenericErrorDefaultFunc(&handler);

    // parse xml from memory
    xmlDocPtr xml_document = xmlParseMemory(content.c_str(), content.length());
    if (xml_document == NULL) {
        raise_error(XML_PARSE_ERROR);
    }

    xmlNode *root_element = NULL;
    root_element = xmlDocGetRootElement(xml_document);

    string file_type = get_feed_type_information(root_element);

    // file type parsing logic
    if (file_type == "Error") {
        raise_error(XML_HEADER_ERROR);
    } else if (file_type == "rss") {
        parse_rss_v2_content(root_element);
    } else if (file_type == "RDF") {
        parse_rss_v1_content(root_element);
    } else if (file_type == "feed") {
        parse_atom_content(root_element);
    } else {
        raise_error(XML_HEADER_ERROR);
    }

    xmlFreeDoc(xml_document);
}


void ConnectionClient::parse_rss_v1_content(xmlNode *root_element) {

    string title = get_feed_title_information(root_element, "rss");
    cout << "*** " << title << " ***" << endl;

    RssItemInformationSummary information = parse_all_rss_items(root_element, false);
    information.print_info(this->has_flag());
}


void ConnectionClient::parse_rss_v2_content(xmlNode *root_element) {

    string title = get_feed_title_information(root_element, "rss");
    cout << "*** " << title << " ***" << endl;

    RssItemInformationSummary information = parse_all_rss_items(root_element, true);    
    information.print_info(this->has_flag());
}


RssItemInformation ConnectionClient::parse_rss_item(xmlNode *item) {

    RssItemInformation information;
    information.title = look_for_property_value("title", item);

    if (this->author_flag) {
        information.author = look_for_property_value("author", item);
        information.dc_creator = look_for_property_value("creator", item);
    }
    if (this->url_flag) {
        string link = look_for_property_value("link", item);
        if (is_url(link))
            information.url = link;
    }
    if (this->updated_flag) {
        information.last_updated = look_for_property_value("pubDate", item);
        information.dc_date = look_for_property_value("date", item);
    }

    return information;
}


RssItemInformationSummary ConnectionClient::parse_all_rss_items(xmlNode *root_node, bool inside_channel) {

    xmlNode *channel = root_node->children;
    xmlNode *top_level_node = NULL;
    xmlNode *temp = NULL;

    // all found results
    RssItemInformationSummary extracted_info;

    // look for items in channel for RSS v2 or in main element for RSS v1
    if (inside_channel) {
        for (top_level_node = channel; top_level_node; top_level_node = top_level_node->next) {
            string name = (char *) top_level_node->name;
            if (name == "channel") {
                channel = top_level_node;
            }
        }
        top_level_node = channel->children;
    } else
        top_level_node = channel;    

    // global information
    extracted_info.dc_creator = look_for_property_value("creator", channel);

    // parse all items
    for (temp = top_level_node; temp; temp=temp->next) {
        if (temp->type == XML_ELEMENT_NODE) {
            string name = (char *) temp->name;
            if (name == "item") {
                extracted_info.item_information.push_back(parse_rss_item(temp));
            }
        }
    }

    return extracted_info;
}


AtomEntryInformation ConnectionClient::parse_atom_entry(xmlNode *item) {

    AtomEntryInformation information;
    information.title = look_for_property_value("title", item);
    
    if (this->author_flag) {
        information.authors = look_for_all_atom_authors(item);
        information.source_authors = look_for_all_source_authors(item);
        information.dc_creators = look_for_all_atom_dc_authors(item);
    }
    if (this->updated_flag) {
        information.last_updated = look_for_property_value("updated", item);
        information.published = look_for_property_value("published", item);
        information.dc_date = look_for_property_value("date", item);
    }
    if (this->url_flag) {
        string link = look_for_atom_link(item, "self");
        if (is_url(link))
            information.url = link;
        link = look_for_atom_link(item, "alternate");
        if (is_url(link))
            information.alternate_url = link;
    }

    return information;
}


AtomEntryInformationSummary ConnectionClient::parse_all_atom_entries(xmlNode *root_element) {

    AtomEntryInformationSummary information;
    xmlNode *feed = root_element->children;
    xmlNode *temp = NULL;

    // get global information
    if (this->author_flag)
        information.global_authors = look_for_all_atom_authors(root_element);

    // parse all entries
    for (temp = feed; temp; temp=temp->next) {
        if (temp->type == XML_ELEMENT_NODE) {
            string name = (char *) temp->name;
            if (name == "entry") {
                information.entry_information.push_back(parse_atom_entry(temp));
            }
        }
    }

    return information;
}


void ConnectionClient::parse_atom_content(xmlNode *root_element) {

    string title = get_feed_title_information(root_element, "atom");
    cout << "*** " << title << " ***" << endl;

    AtomEntryInformationSummary information = parse_all_atom_entries(root_element);
    information.print_info(this->has_flag());
}


// Help functions


string get_feed_type_information(xmlNode *root_node) {
    xmlNode *temp = NULL;
    for (temp = root_node; temp; temp = temp->next) {
        if (temp->type == XML_ELEMENT_NODE) {
            string name = (char *) temp->name;
            return name;
        }
    }
    return "Error";
}


string look_for_attribute(string property_name, xmlNode *item) {
    xmlChar *property_value = xmlGetProp(item, (xmlChar *) property_name.c_str());
    if (property_value == NULL)
        return "";
    string string_prop_value = (char *) property_value;
    return string_prop_value;
}


string get_feed_title_information(xmlNode *root_node, string feed_type) {
    xmlNode *channel = root_node->children;
    xmlNode *top_level_node = NULL;

    if (feed_type == "rss") {
        // find channel element
        for (top_level_node = channel; top_level_node; top_level_node = top_level_node->next) {
            string name = (char *) top_level_node->name;
            if (name == "channel") {
                channel = top_level_node;
            }
        }
        top_level_node = channel->children;
    }
    else
        top_level_node = channel;

    xmlNode *temp;
    for (temp = top_level_node; temp; temp = temp->next) {
        if (temp->type == XML_ELEMENT_NODE) {
            string name = (char *) temp->name;

            if (name == "title") {
                string title = (char *) xmlNodeGetContent(temp);
                return title;
            }
        }
    }
    raise_error(FEED_TITLE_ERROR);
}


string look_for_property_value(string property_name, xmlNode *item) {

    string result = "";
    xmlNode *item_info = item->children;
    xmlNode *temp = NULL;

    for (temp=item_info; temp; temp=temp->next) {
        if (temp->type == XML_ELEMENT_NODE) {
            string name = (char *) temp->name;
            if (name == property_name) {
                result = (char *) xmlNodeGetContent(temp);
                return result;
            }
        }
    }
    return result;
}


xmlNode *look_for_property(string property_name, xmlNode *item) {

    string result = "";
    xmlNode *item_info = item->children;
    xmlNode *temp = NULL;

    for (temp=item_info; temp; temp=temp->next) {
        if (temp->type == XML_ELEMENT_NODE) {
            string name = (char *) temp->name;
            if (name == property_name) {
                return temp;
            }
        }
    }
    return NULL;
}


vector <string> look_for_all_atom_authors(xmlNode *item) {

    vector <string> result;
    xmlNode *item_info = item->children;
    xmlNode *temp = NULL;

    for (temp=item_info; temp; temp=temp->next) {
        if (temp->type == XML_ELEMENT_NODE) {
            string name = (char *) temp->name;

            if (name == "author") {
                string prop_str = look_for_property_value("name", temp);
                if (prop_str != "")
                    result.push_back(prop_str);
            }
        }
    }
    return result;
}


vector <string> look_for_all_source_authors(xmlNode *item) {

    vector <string> result;
    xmlNode *item_info = item->children;
    xmlNode *temp = NULL;

    for (temp=item_info; temp; temp=temp->next) {
        if (temp->type == XML_ELEMENT_NODE) {
            string name = (char *) temp->name;
            if (name == "source") {
                result = look_for_all_atom_authors(temp);
                return result;
            }
        }
    }
    return result;
}


string look_for_atom_link(xmlNode *item, string testing_criterion) {

    string result = "";
    xmlNode *item_info = item->children;
    xmlNode *temp = NULL;

    for (temp=item_info; temp; temp=temp->next) {
        if (temp->type == XML_ELEMENT_NODE) {
            string name = (char *) temp->name;
            if (name == "link") {
                string type = look_for_attribute("rel", temp);
                result = look_for_attribute("href", temp);
                if (type == testing_criterion || (type == "" && testing_criterion == "alternate")) {
                    return result;
                }
            }
        }
    }
    return result;
}


vector <string> look_for_all_atom_dc_authors(xmlNode *item) {

    vector <string> result;
    xmlNode *item_info = item->children;
    xmlNode *temp = NULL;

    for (temp=item_info; temp; temp=temp->next) {
        if (temp->type == XML_ELEMENT_NODE) {
            string name = (char *) temp->name;
            if (name == "creator") {
                string prop_str = (char *) xmlNodeGetContent(temp);
                if (prop_str != "")
                    result.push_back(prop_str);
            }
        }
    }
    return result;
}

bool is_url(string url) {
    if (url.find("http://") == 0)
        return true;
    if (url.find("https://") == 0)
        return true;
    return false;
}
