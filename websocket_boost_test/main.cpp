#define _WIN32_WINNT 0x0601

#include <iostream>
#include "client.hpp"
#include "server.hpp"

#include <locale>
#include <codecvt>

int main() {
    try {
        //std::locale::global(std::locale("uk_UA.UTF-8"));
        //std::locale::global(std::locale("ru_RU.UTF-8"));
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
            //std::string ip = "127.0.0.2";
            //std::string ip = "46.150.85.254";
            std::cout << "\nEnter server: ";
            std::string serv;
            std::cin >> serv;
            std::string ip = "127.0.0.1";
            std::cout << "Client is started: " + serv << std::endl;
            Client client;
            client.connect_socket(serv, 4444);
            client.chat_session();
            //client.echo_session();
        }
        else if (val == 3) {
            //std::string ip = "127.0.0.3";
            std::string ip = "127.0.0.1";
            std::cout << "Client is started: " + ip << std::endl;
            Client client;
            client.connect_socket(ip, 4444);
            client.chat_session();
            //client.echo_session();
        }
        else if (val == 4) {
            //std::string ip = "127.0.0.4";
            std::string ip = "127.0.0.1";
            std::cout << "Client is started: " + ip << std::endl;
            Client client;
            client.connect_socket(ip, 4444);
            client.chat_session();
            //client.echo_session();
        }
        else if (val == 5) {
            //while (true) {
                //std::string ip;
                //std::cin >> ip;
                //std::cout << ip;
            //}

            std::wfstream file("example.txt", std::ios::out | std::ios::in | std::ios::app);
            file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));

            if (file.is_open()) {
                // Запис кирилиці у файл
                file << L"Привіт, світ!" << std::endl;
                file << L"Привет, мир!" << std::endl;
                file << L"Hello world" << std::endl;

                file.close();
            }
            else {
                std::cerr << "Помилка відкриття файлу." << std::endl;
            }
            std::wcout << L"\nДані успішно записано у файл.\n" << std::endl;

            //std::fstream client_chat_file;
            //client_chat_file.open("file.txt", std::ios::in | std::ios::out);

            //if (!client_chat_file.is_open())
                //throw std::runtime_error("Unable to open or create the file client_chat_file.txt");

            //client_chat_file.seekg(0, std::ios::beg);

            //std::string line;
            //while (std::getline(client_chat_file, line)) {
                //std::cout << line << std::endl;
            //}

            //std::cout << "\n\n";

            //if (client_chat_file.fail()) {
                //client_chat_file.clear();
            //}

             // !!!!!!!!!!!!!!!!!!!!!!!!
            //client_chat_file << "\n FAIL FILE\n ";

            //client_chat_file.seekg(0, std::ios::end);
            //client_chat_file.seekg(client_chat_file.tellg(), std::ios::beg);
            //client_chat_file.seekp(-1, std::ios::end);
            //client_chat_file.seekp(0, std::ios::end);
            //client_chat_file.seekp(client_chat_file.tellp(), std::ios::beg);

            //client_chat_file.seekp(0, std::ios::end);

            //for (int i = 0; i < 3; i++) {
                //std::string message;
                //std::cout << "Send: ";
                //std::getline(std::cin, message);

                //std::cout << message + "\n";
                //client_chat_file << message + "\n";
            //}

            //client_chat_file.close();
        }

        system("pause");
        return 0;
    }
    catch (const std::exception &ex) {
        std::cerr << "main error: " << ex.what() << std::endl;
    }
}

//sockets principle
//async 
//break
//console
//echo
//exceptions
//rus ukr
//gui winrt
//aes256
//piblic ip
//Самый простой способ тебе уже сказали - проброс порта на рутере.
//В настройках рутера говоришь : пробросить порт 5555 из WAN интерфейса на порт 22 IP 192.168.0.123 
//(или какой там у тебя IP твоего компа).Потом извне подключаешься на 1.2.3.4 : 5555 
//(где 1.2.3.4 - внешний IP рутера) - профит.
//Еще есть ssh туннели.Но это уже экзотика для специфичных кейсов.
