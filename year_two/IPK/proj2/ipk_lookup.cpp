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
#include <sstream>


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
        return -1;
        //exit(RUNTIME_ERROR);
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
        return "-1";
        //exit(RUNTIME_ERROR);
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

    int get_name(char *buf, unsigned long index, string *str) {
        int current_ind = index;
        int skok = 0;
        while (buf[current_ind] != '\0') {
            bitset<16> bit_field = (unsigned char)buf[current_ind];
            if (bit_field.test(7) && bit_field.test(6)) { // top 11 means reference
                current_ind++;
                bitset<16> address = buf[current_ind]; //load second part of address
                bit_field.set(7, 0); //turn of 11 signs
                bit_field.set(6,0);
                address = address ^ (bit_field << 8);
                int d = get_name(buf, address.to_ulong(), str);
                skok++;
                break;
                //return index;
            }
            else {
                str->push_back(buf[current_ind]);
                current_ind++;
            }
        }
        return current_ind+max(skok,1);
    }

    void print_name(string str, string *name, bool print) {
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
                if (print) {
                    cout << ".";
                }
                *name += '.';
            } else {
                if (index < load_val) {
                    index++;
                    if (print) {
                        cout << str[i];
                    }
                    *name += str[i];
                    i++;
                } else {
                    index = 0;
                    load_num = 1;
                }
            }
        }
        *name += str[i];
        if (print)
            cout << ".";
    }

    int load_query(int index, char *buf) {
        int begin = index;
        while (buf[index] != '\0') {
            index++;
        }
        index += 5;
        return index;
    }

    int print_ipv6(char *buf, unsigned long index, unsigned long data_len, string *name, bool print) {
        struct sockaddr_in6 sa;
        char str[INET6_ADDRSTRLEN];
        char b[16];
        char *addr = (char*)&(sa.sin6_addr);
        for (int i=index; i<index+data_len; i++) {
            addr[i-index] = buf[i];
        }
        inet_ntop(AF_INET6, &(sa.sin6_addr), str, INET6_ADDRSTRLEN);
        if (print) printf("%s", str);
        *name = str;
    }


    int load_answers(unsigned long index, char *buf, bool print, vector<vector<string> > *answers) {
        string s_name;
        index = get_name(buf, index, &s_name);
        //class index 4,5

        bitset<16> types = buf[index+1];
        bitset<16> t1 = buf[index+0];
        types = types ^ (t1 << 8);
        unsigned long types_num = types.to_ulong();
        if (desolve_type(types_num) == "-1") return -1;

        vector <string> retstring;
        string name = "";
        print_name(s_name, &name, print);
        retstring.push_back(name);
        retstring.push_back(desolve_type(types_num));
        if (print) {
            cout << " IN ";
            cout << desolve_type(types_num) << " ";
        }

        bitset<16> len = buf[index+9];
        bitset<16> l1 = buf[index+8];
        len = len ^ (l1 << 8);
        unsigned long data_len = len.to_ulong();
        if (types_num == 1) { //A
            if (print) {
                cout << int((unsigned char)(buf[index+10])) << ".";
                cout << int((unsigned char)(buf[index+11])) << ".";
                cout << int((unsigned char)(buf[index+12])) << ".";
                cout << int((unsigned char)(buf[index+13]));
            }
            string addr = "";
            addr += to_string(int((unsigned char)(buf[index+10]))) + '.';
            addr += to_string(int((unsigned char)(buf[index+11]))) + '.';
            addr += to_string(int((unsigned char)(buf[index+12]))) + '.';
            addr += to_string(int((unsigned char)(buf[index+13]))) + '\0';
            retstring.push_back(addr);
        }
        else if (types_num == 28) { //AAAA
            string addr = "";
            print_ipv6(buf, index+10, data_len, &addr, print);
            retstring.push_back(addr);
        }
        else {
            string name;
            get_name(buf, index+10, &name);
            string retname = "";
            print_name(name, &retname, print);
            retstring.push_back(retname);
        }
        if (print) {
            cout << endl;
        }
        answers->push_back(retstring);
        return index+10+data_len;
    }

    vector<vector<vector<string > > > process_response(char *buff, int desc, bool iterative, bool print) {
        bitset<16> questions = buff[5];
        bitset<16> q1 = buff[4];
        questions = questions ^ (q1 << 8);
        unsigned long questions_num = questions.to_ulong();

        bitset<16> answers = buff[7];
        bitset<16> a1 = buff[6];
        answers = answers ^ (a1 << 8);
        unsigned long answers_num = answers.to_ulong();

        bitset<16> authorities = buff[9];
        bitset<16> au1 = buff[8];
        authorities = authorities ^ (au1 << 8);
        unsigned long auth_num = authorities.to_ulong();
        //cout << "Auth: " << auth_num << endl;

        bitset<16> additional = buff[10];
        bitset<16> ad1 = buff[11];
        additional = additional ^ (ad1 << 8);
        unsigned long add_num = additional.to_ulong();
        //cout << "Add: " << add_num << endl;

        vector<vector<vector<string> > > output;
        unsigned long next = 12;
        for (int j = 0; j<questions_num; j++) { // QUESTION
            next = load_query(next, buff);
        }

        vector<vector <string> > answ;
        for (int j = 0; j<answers_num; j++) { // ANSWERS
            next = load_answers(next, buff, print, &answ);
        }
        output.push_back(answ);

        if (iterative) {
            vector<vector <string> > au;
            for (int j = 0; j<auth_num; j++) { // AUTHORITIES
                next = load_answers(next, buff, false, &au);
            }
            output.push_back(au);
            vector<vector <string> > adda;
            for (int j = 0; j<add_num; j++) { // ADDITIONAL
                next = load_answers(next, buff, false, &adda);
            }
            output.push_back(adda);
        }
        return output;
    }

    //ustanovenie spojenia so serverom
    vector<vector<vector<string > > > establish_connection(bool iterative, bool print) {
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
        vector<vector<vector<string> > > output;
        output = process_response(buff, kolko, iterative, print);
        close(client_socket);
        return output;
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
            if (iterative)
                set_value("rd", 0);
            else
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

            if (type == "PTR") {
                int s;
                unsigned char b[INET6_ADDRSTRLEN];
                if ((s = inet_pton(AF_INET, name.c_str(), b)) > 0) {
                    vector <string> addr;
                    int index = 0;
                    string adrpart;
                    while (index < name.length()) {
                        if (name[index] != '.') {
                            adrpart += name[index];
                        } else {
                            addr.push_back(adrpart);
                            adrpart = "";
                        }
                        index++;
                    }
                    addr.push_back(adrpart);
                    name = addr[3] + "." + addr[2] + "." + addr[1] + "." + addr[0] + ".in-addr.arpa";
                }
                else if ((s = inet_pton(AF_INET6, name.c_str(), b)) > 0) {
                    /*for (int i = 0; i<16; i++) {
                        cout << hex << int(b[i]) << " ";
                    }*/
                    int count = 0;
                    string addr = ""; string microstr = "";

                    int num = 0;
                    int set = 0;

                    for (int i = 0; i<name.length(); i++) {
                        if (name[i]==':') {
                            num++;
                        }
                    }

                    for (int i=name.length()-1; i >= 0; i--) {
                        if (name[i] == ':') {
                            if (set == 1) {
                                for (int j = 0; j<=7-num; j++) {
                                    microstr += "0.0.0.0.";
                                }
                            }
                            while (microstr.length() < 8) {
                                microstr += "0.";
                            }
                            addr += microstr;
                            microstr = "";
                            set = 1;
                        }
                        else {
                            microstr += name[i];
                            microstr += ".";
                            set = 0;
                        }
                    }
                    addr += microstr;
                    addr += "ip6.arpa";
                    name = addr;
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
            if (a != "") {
                if (as==0) {
                    final_name += as + a;
                }
                else {
                    final_name += as + a + char(0);
                }
            }
            if (name[name.length()-1] == '.' && name != ".") {
                final_name += char(0);
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


int iterative_search(string type, string name, string server) {
    vector<vector<vector<string> > > answers;
    //cout << "BEGIN ITER" << endl;
    BIT_DNS_QUERY q(true, "NS", ".");
    Client c(server, q.final_message);
    answers = c.establish_connection(true, false);

    string main_server = "";
    if (answers[0].size()>0) {
        if (answers[0][0].size() > 0) {
            answers[0][0][0].pop_back();
            answers[0][0][0] += ".";
            answers[0][0][2].pop_back();
            answers[0][0][2] += ".";
            cout << answers[0][0][0] << " IN " << answers[0][0][1] << " " << answers[0][0][2] << endl;
            main_server = answers[0][0][2];
        };
    }
    main_server.pop_back();
    //cout << "FOUND MAIN_SERVER " << main_server << endl;

    answers.clear();
    //string main = "j.root-servers.net.";
    BIT_DNS_QUERY q2(true, "A", main_server);
    Client c2(server, q2.final_message);
    answers = c2.establish_connection(true, true);

    string main_server_ip = "";
    if (answers[0].size()>0) {
        if (answers[0][0].size() > 0) {
            main_server_ip = answers[0][0][2];
        };
    }
    //under_server.pop_back();
    //cout << "FOUND MAIN_SERVER_IP " << main_server_ip << endl;
    answers[0].clear();

    while (answers[0].size() == 0) {

        answers.clear();
        BIT_DNS_QUERY q3(true, "NS", name);
        Client c3(main_server_ip, q3.final_message);
        answers = c3.establish_connection(true, false);

        string server = "";
        if (answers[1].size()>0) {
            if (answers[1][0].size() > 0) {
                answers[1][0][0].pop_back();
                answers[1][0][0] += ".";
                answers[1][0][2].pop_back();
                answers[1][0][2] += ".";
                cout << answers[1][0][0] << " IN " << answers[1][0][1] << " " << answers[1][0][2] << endl;
                server = answers[1][0][2];
            };
        }
        server.pop_back();
        //cout << "FOUND SERVER: " << server << endl;

        for (int i=0; i<answers[2].size(); i++) {
            answers[2][0][0].pop_back();
            if (server == answers[2][0][0]) {
                answers[2][0][2].pop_back();
                cout << answers[2][0][0] << " IN " << answers[2][0][1] << " " << answers[2][0][2] << endl;
                main_server_ip = answers[2][0][2];
                break;
            };
        }
        //cout << "FOUND SERVER_IP " << main_server_ip << endl;
        answers.clear();

        BIT_DNS_QUERY q8(true, type, name);
        Client c8(main_server_ip, q8.final_message);
        answers = c8.establish_connection(true, false);
    }
    answers[0][0][0].pop_back();
    answers[0][0][0] += ".";
    answers[0][0][2].pop_back();
    cout << answers[0][0][0] << " IN " << answers[0][0][1] << " " << answers[0][0][2] << endl;

    /*
    //string ip = "192.58.128.30";
    BIT_DNS_QUERY q3(true, "NS", name);
    Client c3(under_server_ip, q3.final_message);
    answers = c3.establish_connection(true);


    /*
    //TODO ziskaj
    //TODO toto bude v cykle dokym sa to nenajde
    string auth = "a.ns.nic.cz.";
    string auth_ip = "194.0.12.1";
    BIT_DNS_QUERY q4(true, "NS", name);
    Client d3(auth_ip, q4.final_message);
    d3.establish_connection(true);
    */
}


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
    if (!iterative) {
        BIT_DNS_QUERY q(iterative, type, name);
        Client c(server, q.final_message);
        c.establish_connection(iterative, true);
    } else {
        iterative_search(type, name, server);
    }


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

