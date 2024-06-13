#define _WIN32_WINNT 0x0601

#include <iostream>
#include "client.hpp"
#include "server.hpp"

#include <locale>
#include <codecvt>

int main() {
    try {
        //std::locale::global(std::locale("uk_UA.UTF-8"));
        //setlocale(LC_ALL, "");
        system("chcp 65001");

        size_t val = 0;
        std::cout << "Enter value: ";
        std::cin >> val;
        std::cout << std::endl;

        if (val == 1) {
            std::cout << "Server is started: " << std::endl;
            Server server;
            server.init_acceptor();
            server.listen_server(&Server::chat_session);
            //server.listen_server(&Server::echo_session);
        }
        else if (val == 2) {
            std::cout << "\nEnter server: "; // "127.0.0.1"
            std::string serv;
            std::cin >> serv;
            std::cout << "Client is started: " + serv << std::endl;
            Client client;
            client.connect_socket(serv, 4444);
            client.chat_session();
            //client.echo_session();
        }
        else if (val == 3) {
            std::string ip = "127.0.0.3";
            std::cout << "Client is started: " + ip << std::endl;
            Client client;
            client.connect_socket(ip, 4444);
            client.chat_session();
            //client.echo_session();
        }
        else if (val == 4) {
            std::string ip = "127.0.0.4";
            std::cout << "Client is started: " + ip << std::endl;
            Client client;
            client.connect_socket(ip, 4444);
            client.chat_session();
            //client.echo_session();
        }
        system("pause");
        return 0;
    }
    catch (const std::exception &ex) {
        std::cerr << "main error: " << ex.what() << std::endl;
    }
}
