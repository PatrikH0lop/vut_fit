// file:   ipk_proj1_server.cpp
// author: Patrik Holop, xholop01
// about:  Serverova cast projektu


//kniznice
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>


//programove konstanty
#define BUFSIZE 256
#define ARG_ERR 1
#define INIT_VALUE -1
#define INIT_CHAR 'z'
#define ERROR 1


using namespace std;
typedef struct sockaddr_in sockaddr_in;
//vstupny subor
ifstream input_file;


//Trieda server reprezentujuca serverovu cast aplikacie
class Server {

    private:
        int port;
        int server_socket;
        sockaddr_in server_address, client_address;

    public:
        Server(int port) {
            this->port = port;
            this->server_socket = INIT_VALUE;
        }

    // Najde podretazec v tabulkovom retazci (x:y:z...) na zadanej pozicii
    void find_substr(int position, string str, char *buf) {
        string substr = "";
        int index = 0, counter = 0;
        char ch = str[0];

        while (ch != '\0') {
            if (ch == ':') {
                counter += 1;
            } else if (counter == position) {
                substr += str[index];
            }
            index += 1;
            ch = str[index];
        }
        fill_n(buf, BUFSIZE, 0);
        strcpy(buf, substr.c_str());
    }

    // Ustanovi spojenie a v pripade pripojenia obsluhuje klientov
    void establish_connection() {
        char buffer[BUFSIZE];
        fill_n(buffer, BUFSIZE, 0);

        //vytvorenie serveroveho socketu
        int init_socket = INIT_VALUE;
        if ((init_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
            cerr << "Error while creating socket" << endl;
            exit(ERROR);
        }

        //ustanovenie zakladnych atributov
        socklen_t client_address_len = sizeof(client_address);
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = htonl(INADDR_ANY);
        server_address.sin_port = htons((unsigned short)this->port);

        //spojenie a pocuvanie
        int return_code = 0;
        if ((return_code = bind(init_socket, (struct sockaddr*)&server_address, sizeof(server_address))) < 0)
        {
            cerr << "Error while binding" << endl;
            exit(ERROR);
        }

        if ((listen(init_socket, 1)) < 0) {
            cerr << "Error while listening at init_socket" << endl;
            exit(ERROR);
        }

        int state, res, desc;
        string login = "";
        char option = INIT_CHAR;

        //hladanie klienta a jeho obsluha
        while(true) {
            int open_socket = accept(init_socket, (struct sockaddr*)&client_address, &client_address_len);
            if (open_socket > 0) {
                //vytvorenie samostatneho procesu pre klienta
                pid_t child_process = fork();
                if (child_process == 0) {

                    state = 0;
                    //konecny automat
                    while (true) {
                        fill_n(buffer, BUFSIZE, 0);
                        res = recv(open_socket, buffer, BUFSIZE, 0);

                        //nacitanie loginu
                        if (state == 0) {
                            login = buffer;
                            login.erase(0, 6);
                            fill_n(buffer, BUFSIZE, 0);
                            strcpy(buffer, "LOGIN_OK");
                            desc = send(open_socket, buffer, strlen(buffer), 0);
                            if (desc < 0) {
                                cerr << "Error while sending login ack" << endl;
                                break;
                            }
                            state += 1;
                          // nacitanie moznosti
                        } else if (state == 1) {
                            string opt = buffer;
                            opt.erase(0, 7);
                            option = opt[0];
                            fill_n(buffer, BUFSIZE, 0);
                            strcpy(buffer, "OPTION_OK");
                            desc = send(open_socket, buffer, strlen(buffer), 0);
                            if (desc < 0) {
                                cerr << "Error while sending option ack" << endl;
                            }
                            state += 1;
                          // posielanie sprav
                        } else if (state == 2) {
                            string data_trigger = "SEND_INFO";
                            if (strcmp(buffer, data_trigger.c_str()) != 0) {
                                cerr << "Expected request for data from client" << endl;
                                break;
                            }

                            fill_n(buffer, BUFSIZE, 0);
                            //poslanie informacii o uzivatelovi
                            if (option == 'n') {
                                string foundline = find_user(login);
                                if (foundline != "NONE") {
                                    find_substr(4, foundline, buffer);
                                } else {
                                    strcpy(buffer, "NONE");
                                }
                                desc = send(open_socket, buffer, strlen(buffer), 0);
                                if (desc < 0) {
                                    cerr << "Error sending data" << endl;
                                }
                                break;
                                // poslanie domovskeho adresa
                            } else if (option == 'f') {
                                string foundline = find_user(login);
                                if (foundline != "NONE") {
                                    find_substr(5, foundline, buffer);
                                } else {
                                    strcpy(buffer, "NONE");
                                }
                                desc = send(open_socket, buffer, strlen(buffer), 0);
                                if (desc < 0) {
                                    cerr << "Error sending data" << endl;
                                }
                                break;
                                // poslanie vsetkych loginov
                            } else if (option == 'l') {
                                vector<string> foundlines = find_users(login);
                                for (string foundline: foundlines) {
                                    find_substr(0, foundline, buffer);
                                    desc = send(open_socket, buffer, strlen(buffer), 0);
                                    if (desc < 0) {
                                        cerr << "Error sending data " << endl;
                                    }
                                    fill_n(buffer, BUFSIZE, 0);
                                    res = recv(open_socket, buffer, BUFSIZE, 0);
                                    if (res <= 0 || strcmp(buffer, "SEND_INFO") != 0) {
                                        cerr << "Expected cerr info" << endl;
                                        break;
                                    }
                                }
                                fill_n(buffer, BUFSIZE, 0);
                                strcpy(buffer, "END\0");
                                desc = send(open_socket, buffer, strlen(buffer), 0);
                                if (desc < 0) {
                                    cerr << "Error while sending END signal" << endl;
                                }
                                break;
                            }
                        }

                        if (res <= 0) {
                            cerr << "Connection lost" << endl;
                            break;
                        };
                    }
                    // ukoncenie klientskeho procesu
                    close(open_socket);
                    exit(0);
                } else if (child_process < 0) {
                    cerr << "Warning, child process not created" << endl;
                }
            };
            // ukoncenie hlavneho procesu
            close(open_socket);
        }
    }

    // vyhlada uzivatela v subore
    string find_user(string name) {
        string line;
        char tmp[BUFSIZE];
        input_file.open("/etc/passwd");

        while(getline(input_file, line)) {
            fill_n(tmp, BUFSIZE, 0);
            find_substr(0, line, tmp);
            if (strcmp(tmp, name.c_str()) == 0) {
                input_file.close();
                return line;
            }
        }
        input_file.close();
        return "NONE";
    }

    // vyhlada uzivatelov v subore
    vector<string> find_users(string name) {
        string line;
        vector<string> arr;
        input_file.open("/etc/passwd");
        while(getline(input_file, line)) {
            if (name == "no_login") {
                arr.push_back(line);
            } else if (line.find(name.c_str()) == 0) {
                arr.push_back(line);
            }
        }
        input_file.close();
        return arr;
    }
};


//kontrola parametrov
void check_params(int argc, char *argv[], int *port) {
    int arg;
    int count = 0;
    if (argc == 1) {
        cerr << "Missing option -p" << endl;
        exit(ERROR);
    }
    while ((arg=getopt(argc, argv, "p:"))!=-1) {
        switch(arg) {
            case 'p':
                if (optarg) {
                    if (count == 0) {
                        char *error;
                        *port = strtol(optarg, &error, 10);
                        if (*error != '\0') {
                            cerr << "Please check -p option, conversion error" << endl;
                            exit(ERROR);
                        }
                        count = 1;
                    }
                    else {
                        cerr << "Duplicated option -p" << endl;
                        exit(ERROR);
                    }
                }
                else {
                    cerr << "Missing value of option -p" << endl;
                    exit(ERROR);
                }
                break;
            default:
                cerr << "Unknown option" << endl;
                exit(ERROR);
        }
    }
    if (optind < argc) {
        cerr << "Too much options" << endl;
        exit(ERROR);
    }
}


//hlavny program
int main(int argc, char *argv[]) {
    int port = -1;
    check_params(argc, argv, &port);
    Server server = Server(port);
    server.establish_connection();
}
