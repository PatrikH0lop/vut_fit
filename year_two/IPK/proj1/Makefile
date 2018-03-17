all: ipk-client ipk-server
ipk-client: ipk_proj1_client.cpp
	g++ -g -o ipk-client -static-libstdc++ ./ipk_proj1_client.cpp
ipk-server: ipk_proj1_server.cpp
	g++ -g -std=c++11 -o ipk-server -static-libstdc++ ./ipk_proj1_server.cpp
