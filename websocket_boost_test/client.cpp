#define _WIN32_WINNT 0x0601

#include "client.hpp"
#include <iostream>
#include <cwctype>
#include <iomanip>
//#include <chrono>

void Client::connect_socket(const std::string client_ip, const boost::asio::ip::port_type port) {
    try {
        boost::asio::ip::tcp::socket socket(io_context);
        socket.connect(boost::asio::ip::tcp::endpoint(
            boost::asio::ip::make_address(client_ip), port));
            //boost::asio::ip::address::from_string(client_ip), port

        std::cout << "\nCurrent IP: " << socket.local_endpoint().address().to_string() << std::endl;
        std::cout << "\nConnection from: " << socket.remote_endpoint().address().to_string() << std::endl;

        client_chat_file.open(socket.remote_endpoint().address().to_string() + "_client_chat.txt",
                              std::ios::in | std::ios::out | std::ios::app);

        ws.next_layer() = std::move(socket);
        ws.binary(true);
        //ws.handshake("127.0.0.1", "/"); // Handshake
        ws.handshake(client_ip, "/"); // Handshake
    }
    catch (const std::exception &ex) {
        std::cerr << "Connection socket error: " << ex.what() << std::endl;
        return;
    }
}

void Client::echo_session() {
    try {
        while (true) {
            std::string message;
            std::cout << "Enter message: ";
            std::getline(std::cin, message);

            ws.write(boost::asio::buffer(message));

            buffer.consume(buffer.size());
            ws.read(buffer);
            std::cout << "Received: " << boost::beast::buffers_to_string(buffer.data()) << std::endl;
        }
    }
    catch (const std::exception &ex) {
        std::cerr << "WebSocket closed. Reason: " << ws.reason().reason << std::endl;
        std::cerr << "Init acceptor error: " << ex.what() << std::endl;
        return;
    }
}

void Client::chat_session() {
    try {
        if (!client_chat_file.is_open())
            throw std::runtime_error("Unable to open or create the file client_chat.txt");
        std::cout << "\nPrivate Chat:\n" << std::endl;

        std::string message_get;
        client_chat_file.seekg(0, std::ios::beg);
        while (std::getline(client_chat_file, message_get)) {
            if (message_get.at(0) == ' ')
                std::cout << std::setw(28) << "(Received): " << message_get << std::endl;
            else 
                std::cout << message_get << std::endl;
        }
        message_get.clear();
        std::cout << "\nLive Chat:\n" << std::endl;

        if (client_chat_file.fail())
            client_chat_file.clear();
        client_chat_file.seekp(0, std::ios::end);

        std::thread data_server_thread = std::thread(&Client::listen_data_server, this);

        while (true) {
            std::getline(std::cin, message_send);

            for (const char c : message_send) {
                if (!std::iswspace(c)) {
                    size_t space_pos = message_send.find(c);
                    if (std::string::npos == space_pos)
                        throw std::logic_error("Impossible to cut spaces");
                    message_send = message_send.substr(space_pos);

                    //std::vector<uint8_t> bin_message(message.begin(), message.end);
                    std::unique_lock<std::mutex> lock(mutex_chat_file);
                    if (ws.is_open())
                        ws.write(boost::asio::buffer(message_send));
                    else
                        break;
                    if (message_send == "1111")
                        break;

                    client_chat_file << message_send << std::endl;
                    break;
                }
            }
            if (!ws.is_open() || message_send == "1111")
                break;
        }

        if (data_server_thread.joinable())
            data_server_thread.join();

        std::cout << "\n***** GOODBYE CLIENT *****" << std::endl;
        std::cout << "WebSocket closed. Reason: " << ws.reason().code << std::endl;
    }
    catch (const std::exception &ex) {
        std::cerr << "WebSocket closed. Reason: " << ws.reason().code << std::endl;
        std::cerr << "Init acceptor error: " << ex.what() << std::endl;
        return;
    }
}

void Client::listen_data_server() {
    try {
        if (!client_chat_file.is_open())
            throw std::runtime_error("Unable to open or create the file client_chat.txt");
        std::string buffer_data;
        while (true) {
            buffer.consume(buffer.size());
            if (ws.read(buffer) > 0) {
                buffer_data = boost::beast::buffers_to_string(buffer.data());
                if (buffer_data == "1111")
                    return;
                std::unique_lock<std::mutex> lock(mutex_chat_file);
                std::cout << std::setw(28) << "(Received): " << buffer_data << std::endl;
                client_chat_file << " " << buffer_data << std::endl;
            }
            if (message_send == "1111")
                return;
        }
    }
    catch (const std::exception &ex) {
        std::cerr << "listen data server error: " << ex.what() << std::endl;
        return;
    }
}
