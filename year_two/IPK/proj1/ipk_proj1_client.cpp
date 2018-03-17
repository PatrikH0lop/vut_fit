// file:   ipk_proj1_client.cpp
// author: Patrik Holop, xholop01
// about:  Klientska cast projektu


//kniznice
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>


//programove konstanty
#define BUFSIZE 256
#define INIT_NUM -1
#define INIT_CHAR 'z'
#define ERROR 1


using namespace std;
typedef struct sockaddr_in sockaddr_in;


//Trieda reprezentujuca klientsku cast
class Client {

    private:
        int port;
        string server_name, login;
        char opt;
        int client_socket;
        sockaddr_in server_address;

    public:
        Client(int port, string server_name, char opt, string login) {
            this->port = port;
            this->server_name = server_name;
            this->opt = opt;
            this->client_socket = INIT_NUM;
            this->login = login;
            bzero((char *) &server_address, sizeof(server_address));
        }

        //otestovanie poslednej odpovede pre pripadne chyby
        void test_communication(int comm_desc, string message) {
            if (comm_desc < 0) {
                cerr << message << " failed" << endl;
                close(client_socket);
                exit(ERROR);
            }
        }

        //poslanie a overenie loginu
        void send_login(char *buf) {
            fill_n(buf, BUFSIZE, 0);
            string log = "LOGIN " + this->login;
            strcpy(buf, log.c_str());

            int desc = send(this->client_socket, buf, strlen(buf), 0);
            test_communication(desc, "sending login");

            fill_n(buf, BUFSIZE, 0);
            desc = recv(client_socket, buf, BUFSIZE, 0);
            test_communication(desc, "receiving logic ack");

            if (strcmp(buf, "LOGIN_OK") != 0) {
                cerr << "receiving login ack failed" << endl;
                close(client_socket);
                exit(ERROR);
            }
        }

        //poslanie a overenie moznosti
        void send_option(char *buf) {
            fill_n(buf, BUFSIZE, 0);
            string option = "OPTION ";
            option += this->opt;
            strcpy(buf, option.c_str());

            int desc = send(this->client_socket, buf, strlen(buf), 0);
            test_communication(desc, "sending option");

            fill_n(buf, BUFSIZE, 0);
            desc = recv(client_socket, buf, BUFSIZE, 0);
            test_communication(desc, "receiving option ack");
            if (strcmp(buf, "OPTION_OK") != 0) {
                cerr << "verifying option ack failed" << endl;
                close(client_socket);
                exit(ERROR);
            }
        }

        //vyziadanie dat od serveru
        void request_data(char *buf) {
            fill_n(buf, BUFSIZE, 0);
            strcpy(buf, "SEND_INFO");

            int desc = send(this->client_socket, buf, strlen(buf), 0);
            test_communication(desc, "sending request for data");

            fill_n(buf, BUFSIZE, 0);
            desc = recv(client_socket, buf, BUFSIZE, 0);
            test_communication(desc, "receiving data");
        }

        //ustanovenie spojenia so serverom
        void establish_connection() {
            char buffer[BUFSIZE];
            fill_n(buffer, BUFSIZE, 0);

            //vytvorenie socketu
            if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
                cerr <<  "Error while creating socket" << endl;
                close(client_socket);
                exit(ERROR);
            }

            struct hostent *server;
            if ((server = gethostbyname(this->server_name.c_str())) == NULL) {
                cerr << "Error, server not found: " << this->server_name << endl;
                close(client_socket);
                exit(ERROR);
            }

            //nastavenie zakladnych atributov
            server_address.sin_family = AF_INET;
            bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr,
                  server->h_length);
            server_address.sin_port = htons(this->port);
            socklen_t serverlen = sizeof(server_address);

            if (connect(client_socket, (const struct sockaddr *)&server_address, sizeof(server_address))!=0) {
                cerr << "Error, client can't connect to server" << endl;
                close(client_socket);
                exit(ERROR);
            }

            int bytestx, bytesrx;

            //uvodna komunikacia poslanim loginu a moznosti
            send_login(buffer);
            send_option(buffer);

            //na zaklade moznosti vyziadanie dat
            if (this->opt == 'n') {
                request_data(buffer);
                if (strcmp(buffer, "NONE") != 0) {
                    cout << buffer << endl;
                } else {
                    cerr << "Nonexisting login: " << buffer << endl;
                }
            }
            else if (this->opt == 'f') {
                request_data(buffer);
                if (strcmp(buffer, "NONE") != 0) {
                    cout << buffer << endl;
                } else {
                    cerr << "Nonexisting login: " << buffer << endl;
                }
            } else if (this->opt == 'l') {
                string ending_phrase = "END";
                bool finish = false;

                while (!finish) {
                    request_data(buffer);
                    if (strcmp(buffer, ending_phrase.c_str()) == 0) {
                        finish = true;
                    } else {
                        cout << buffer << endl;
                    }
                }
            }
        close(client_socket);
     }
};


//kontrola parametrov
void check_params(int argc, char *argv[], string *host, int *port, char *opt, string *login) {
    int arg;
    bool h_set = false, p_set = false;

    while ((arg=getopt(argc, argv, "h:p:nfl"))!=-1) {
        switch(arg) {
            case 'h':
                if (optarg && !h_set) {
                    *host = optarg;
                    h_set = true;
                } else {
                    cerr << "Please check -h option, wrong shape" << endl;
                    exit(ERROR);
                }
              break;

            case 'p':
                if (optarg && !p_set) {
                    char *error;
                    p_set = true;
                    *port = strtol(optarg, &error, 10);
                    if (*error != '\0') {
                        cerr << "Please check -p option, conversion error" << endl;
                        exit(ERROR);
                    }
                } else {
                    cerr << "Please check -p option, wrong shape" << endl;
                    exit(ERROR);
                }
                break;

            case 'n':
                if (*opt == INIT_CHAR) {
                    *opt = 'n';
                } else {
                    cerr << "Please check -n option, arg_error" << endl;
                    exit(ERROR);
                }
              break;

            case 'f':
                if (*opt == INIT_CHAR) {
                    *opt = 'f';
                }
                else {
                    cerr << "Please check -f option, arg_error" << endl;
                    exit(ERROR);
                }
                break;

            case 'l':
                if (*opt == INIT_CHAR) {
                    *opt = 'l';
                }
                else {
                    cerr << "Please check -l option, arg_error" << endl;
                    exit(ERROR);
                }
                break;

            default:
                cerr << "Wrong usage of arguments, arg_error" << endl;
                exit(ERROR);
        }
    }

    if ((argc-optind != 1 && *opt!='l') || argc == 1) {
        cerr << "Wrong usage of arguments, arg_error" << endl;
        exit(ERROR);
    }

    if (*opt!='l') {
        *login = argv[optind];
    } else if (argc-optind == 1) {
        *login = argv[optind];
    } else if (argc-optind > 1) {
        cerr << "Too many arguments, arg_error" << endl;
        exit(ERROR);
    } else {
      *login = "no_login";
    }
}


//hlavny program
int main(int argc, char *argv[]) {
    string host = "";
    string login = "";
    int port = INIT_NUM;
    char opt = INIT_CHAR;
    check_params(argc, argv, &host, &port, &opt, &login);
    Client client = Client(port, host, opt, login);
    client.establish_connection();
    return 0;
}
