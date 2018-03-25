// file:   ipk_dns_client.cpp
// author: Patrik Holop, xholop01
// about:  Klient dotazujuci sa na DNS


//kniznice
#include <vector>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <bitset>
#include <map>
#include <arpa/inet.h>


//programove konstanty
#define ERROR 2
#define RUNTIME_ERROR 1

#define BUFSIZE 512

using namespace std;
typedef struct sockaddr_in sockaddr_in;

int resolve_type(string type) {
    if (type == "PTR")
        return 12;
    else if (type == "A")
        return 1;
    else if (type == "AAAA")
        return 28;
    else if (type == "NS")
        return 2;
    else if (type == "CNAME")
        return 5;
    else {
        cerr << "unknown type" << endl;
        exit(RUNTIME_ERROR);
    }
}

string desolve_type(unsigned long type) {
    if (type == 12)
        return "PTR";
    else if (type == 1)
        return "A";
    else if (type == 28)
        return "AAAA";
    else if (type == 2)
        return "NS";
    else if (type == 5)
        return "CNAME";
    else {
        cerr << "unknown type" << endl;
        exit(RUNTIME_ERROR);
    }
}

void check_params(int argc, char *argv[], string *server, int *timeout, string *type, bool *iter, string *name);
//Trieda reprezentujuca klientsku cast
class Client {
private:
    int port = 53;
    string server_name, message;
    int client_socket;
    sockaddr_in server_address;

public:
    Client(string server_name, string message) {
        this->server_name = server_name;
        this->client_socket = -1;
        this->message = message;
        bzero((char *) &server_address, sizeof(server_address));
    }

    //otestovanie poslednej odpovede pre pripadne chyby
    void test_communication(int comm_desc, string message) {
        if (comm_desc < 0) {
            cerr << message << " failed" << endl;
            close(client_socket);
            exit(RUNTIME_ERROR);
        }
    }

    //poslanie a overenie loginu
    int send_query(char *buf, char *buff) {
        //endto(s,(char*)buf,sizeof(struct DNS_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION),0,(struct sockaddr*)&dest,sizeof(dest)) < 0)
        int desc = sendto(this->client_socket, message.c_str(), message.length(), 0, (struct sockaddr*)&server_address, sizeof(server_address));
        test_communication(desc, "sending message");
        fill_n(buf, BUFSIZE, 0);
        int dsa = sizeof server_address;
        fill_n(buff, 65000, 0);
        desc = recvfrom(client_socket, (char*)buff, 65000, 0, (struct sockaddr*)&server_address, (socklen_t*)&dsa);
        test_communication(desc, "receiving message ack");
        return desc;
    }

    void get_name(char *buf, unsigned long index, string *str) {
        int current_ind = index;
        while (buf[current_ind] != '\0') {
            bitset<16> bit_field = (unsigned char)buf[current_ind];
            if (bit_field.test(7) && bit_field.test(6)) { // top 11 means reference
                current_ind++;
                bitset<16> address = buf[current_ind]; //load second part of address
                bit_field.set(7, 0); //turn of 11 signs
                bit_field.set(6,0);
                address = address ^ (bit_field << 8);
                get_name(buf, address.to_ulong(), str);
                return;
            }
            else {
                str->push_back(buf[current_ind]);
                current_ind++;
            }
        }
    }

    void print_name(string str) {
        int load_num = 1;
        int load_val = 0;
        int first = 1;
        int index = 0;
        int i = 0;

        while (i < str.length()) {
            if (load_num == 1) {
                load_val = str[i];
                load_num = 0;
                i++;
                if (first == 1) {
                    first = 0;
                    continue;
                }
                cout << ".";
            } else {
                if (index < load_val) {
                    index++;
                    cout << str[i];
                    i++;
                } else {
                    index = 0;
                    load_num = 1;
                }
            }
        }
    }

    int load_query(int index, char *buf) {
        int begin = index;
        while (buf[index] != '\0') {
            index++;
        }
        index += 5;
        return index;
    }

    int load_answers(unsigned long index, char *buf) {
        string s_name;
        get_name(buf, index, &s_name);
        print_name(s_name);
        //class index 4,5
        cout << ". IN ";

        bitset<16> types = buf[index+3];
        bitset<16> t1 = buf[index+2];
        types = types ^ (t1 << 8);
        unsigned long types_num = types.to_ulong();
        cout << desolve_type(types_num) << " ";

        bitset<16> len = buf[index+11];
        bitset<16> l1 = buf[index+10];
        len = len ^ (l1 << 8);
        unsigned long data_len = len.to_ulong();

        if (types_num == 1) { //A
            cout << int((unsigned char)(buf[index+12])) << ".";
            cout << int((unsigned char)(buf[index+13])) << ".";
            cout << int((unsigned char)(buf[index+14])) << ".";
            cout << int((unsigned char)(buf[index+15]));
        }
        else {
            string name;
            get_name(buf, index+12, &name);
            print_name(name);
        }
        cout << endl;
        return index+12+data_len;
    }

    void process_response(char *buff, int desc) {
        bitset<16> questions = buff[5];
        bitset<16> q1 = buff[4];
        questions = questions ^ (q1 << 8);
        unsigned long questions_num = questions.to_ulong();

        bitset<16> answers = buff[7];
        bitset<16> a1 = buff[6];
        answers = answers ^ (a1 << 8);
        unsigned long answers_num = answers.to_ulong();

        unsigned long next = 12;
        for (int j = 0; j<questions_num; j++) {
            next = load_query(next, buff);
        }
        for (int j = 0; j<answers_num; j++) {
            next = load_answers(next, buff);
        }
    }

    //ustanovenie spojenia so serverom
    void establish_connection() {
        char buffer[BUFSIZE];
        fill_n(buffer, BUFSIZE, 0);

        //vytvorenie socketu
        if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) <= 0) {
            cerr <<  "Error while creating socket" << endl;
            close(client_socket);
            exit(RUNTIME_ERROR);
        }

        //nastavenie zakladnych atributov
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr(server_name.c_str());
        server_address.sin_port = htons(this->port);
        socklen_t serverlen = sizeof(server_address);

        int bytestx, bytesrx;

        //uvodna komunikacia poslanim loginu a moznosti
        char buff[65000];
        int kolko = send_query(buffer, buff);
        process_response(buff, kolko);

        close(client_socket);
    }
};


class BIT_DNS_HEADER {
    public:
        bitset<96> header;
        map<string, int> access_list;

        BIT_DNS_HEADER() {
            access_list.insert(pair<string,int>("id", 80));
            access_list.insert(pair<string,int>("qr", 79));
            access_list.insert(pair<string,int>("opcode", 75));
            access_list.insert(pair<string,int>("aa", 74));
            access_list.insert(pair<string,int>("tc", 73));
            access_list.insert(pair<string,int>("rd", 72));
            access_list.insert(pair<string,int>("ra", 71));
            access_list.insert(pair<string,int>("res", 68));
            access_list.insert(pair<string,int>("rcode", 64));
            access_list.insert(pair<string,int>("questions", 48));
            access_list.insert(pair<string,int>("answ_rrs", 32));
            access_list.insert(pair<string,int>("auth_rrs", 16));
            access_list.insert(pair<string,int>("addi_rrs", 0));
        }

        void set_value(string sel, int value) {
            int offset = access_list.find(sel)->second;
            bitset<96> new_value = value;
            header = header ^ (new_value << offset);
        }

        void clear_header() {
            this->header = this->header ^ this->header;
        }

        void print_header() {
            cout << hex << header << endl;
        }
};

class BIT_DNS_QUERY: BIT_DNS_HEADER {
    public:
        string encoded_name;
        string encoded_postfix;
        string encoded_header;
        string final_message;

        BIT_DNS_QUERY(bool iterative, string type, string name) {
            string str;
            for (auto ch: name) {
                str += bitset<8>(ch).to_string();
            }
            this->encoded_name = str;
            bitset<32> postfix = 1;
            bitset<32> _type = resolve_type(type);
            postfix = postfix ^ (_type << 16);

            this->encoded_postfix = postfix.to_string();

            set_value("id", getpid());
            set_value("qr", 0);
            set_value("opcode", 0);
            set_value("aa", 0);
            set_value("tc", 0);
            set_value("rd", 1);
            set_value("ra", 0);
            set_value("set", 0);
            set_value("rcode", 0);
            set_value("questions", 1);
            set_value("answ_rrs", 0);
            set_value("auth_rrs", 0);
            set_value("addi_rrs", 0);
            this->encoded_header = header.to_string();
            this->final_message = this->encoded_header + this->encoded_name + this->encoded_postfix;
            string strin;
            string prestra;
            for (int i = 0; i<encoded_header.size(); i++) {
                strin += encoded_header[i];
                if (strin.length() == 8) {
                    bitset<8> bata = bitset<8>(strin);
                    unsigned long a = bata.to_ulong();
                    unsigned char c = static_cast<unsigned char>(a);
                    prestra += c;
                    strin.resize(0);
                }
            }

            char as = 0;
            string a = "";
            string final_name = "";
            for (int i = 0; i<name.length(); i++) {
                if (name[i] == '.') {
                    final_name += as + a;
                    as = 0;
                    a = "";
                } else {
                    a += name[i];
                    as++;
                }
            }
            if (as==0) {
                final_name += as + a;
            }
            else {
                final_name += as + a + char(0);
            }

            string postra;
            strin = "";
            for (int i = 0; i<encoded_postfix.size(); i++) {
                strin += encoded_postfix[i];
                if (strin.length() == 8) {
                    bitset<8> bata = bitset<8>(strin);
                    unsigned long a = bata.to_ulong();
                    unsigned char c = static_cast<unsigned char>(a);
                    postra += c;
                    strin.resize(0);
                }
            }
            this->final_message = prestra + final_name + postra;
        }
};


//hlavny program
int main(int argc, char *argv[]) {
    string server = "";
    string name = "";
    string type = "A";
    bool iterative = false;
    int timeout = 5;
    check_params(argc, argv, &server, &timeout, &type, &iterative, &name);

    //DNS_QUERY new_query(type, iterative, name);
    //new_query.to_string();
    //new_query.send_query();

    BIT_DNS_QUERY q(iterative, type, name);
    Client c(server, q.final_message);
    c.establish_connection();

    //Client client = Client(port, host, opt, login);
    //client.establish_connection();
    return 0;
}


//kontrola parametrov
void check_params(int argc, char *argv[], string *server, int *timeout, string *type, bool *iter, string *name) {
    int arg;
    bool server_set = false, timeout_set = false, type_set = false, iter_set = false, login_set = false, help_set = false;

    while ((arg=getopt(argc, argv, "hs:T:t:i")) != -1) {
        switch(arg) {
            case 'h':
                if (!help_set) {
                    help_set = true;
                }
                else {
                    cerr << "Too many -h options";
                    exit(ERROR);
                }
                break;
            case 's':
                if (optarg && !server_set) {
                    *server = optarg;
                    server_set = true;
                } else {
                    cerr << "Please check -h option, wrong shape" << endl;
                    exit(ERROR);
                }
                break;

            case 'T':
                if (optarg && !timeout_set) {
                    char *error;
                    timeout_set = true;
                    *timeout = strtol(optarg, &error, 10);
                    if (*error != '\0') {
                        cerr << "Please check -T option, conversion error" << endl;
                        exit(ERROR);
                    }
                } else {
                    cerr << "Please check -T option, wrong shape" << endl;
                    exit(ERROR);
                }
                break;

            case 't':
                if (optarg && !type_set) {
                    *type = optarg;
                    type_set = true;
                } else {
                    cerr << "Please check -t option, arg_error" << endl;
                    exit(ERROR);
                }
                break;

            case 'i':
                if (!iter_set) {
                    *iter = true;
                    iter_set = true;
                }
                else {
                    cerr << "Please check -i option, arg_error" << endl;
                    exit(ERROR);
                }
                break;

            default:
                cerr << "Wrong usage of arguments, arg_error" << endl;
                exit(ERROR);
        }
    }

    if ((argc-optind != 1 && help_set == false) || (help_set && (argc-optind) != 0) || argc == 1 || (server_set == false && help_set == false)) {
        cerr << "Wrong usage of arguments, arg_error" << endl;
        exit(ERROR);
    }
    if (help_set && !(server_set || type_set || type_set || iter_set)) {
        cout << "Usage\n ./ipk-lookup [-h]\n ./ipk-lookup -s server [-T timeout] [-t type] [-i] name" << endl;
        exit(0);
    } else if (help_set) {
        cerr << "Help -h must be set alone" << endl;
        exit(ERROR);
    }
    *name = argv[optind];
}

