#define _WIN32_WINNT 0x0601

#include "server.hpp"
#include <iostream>
#include <cwctype>
#include <iomanip>
//#include <chrono>

void Server::init_acceptor() {
    try {
        acceptor.open(boost::asio::ip::tcp::v4());
        acceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 4444));
        acceptor.listen();
    }
    catch (const std::exception &ex) {
        std::cerr << "Init acceptor error: " << ex.what() << std::endl;
        return;
    }
}

void Server::listen_server(void (Server:: *session)(boost::asio::ip::tcp::socket &socket)) {
    try {
        while (true) {
            boost::asio::ip::tcp::socket _socket(io_context);
            acceptor.accept(_socket);
            std::thread(std::bind(session, this, std::move(_socket))).detach();
            //std::thread(std::bind(session, this, std::placeholders::_1), std::move(socket)).detach();
            //std::thread(&Server::echo_session, this).detach();
        }
    }
    catch (const std::exception &ex) {
        std::cerr << "Thread error: " << ex.what() << std::endl;
        return;
    }
}

void Server::echo_session() {
    try {
        boost::asio::ip::tcp::socket socket(io_context);
        acceptor.accept(socket);

        std::cout << "\nConnection from: " << socket.remote_endpoint().address().to_string() << std::endl;
        std::cout << "Current IP: " << socket.local_endpoint().address().to_string() << std::endl;

        boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws(std::move(socket));

        ws.accept(); // handshake

        // Loop for handling messages
        boost::beast::flat_buffer buffer;
        while (true) {
            buffer.consume(buffer.size());
            ws.read(buffer);
            ws.write(buffer.data());
        }

        boost::beast::websocket::close_reason cr;
        cr.reason = "Goodbye"; // close reason
        ws.close(cr);
    }
    catch (const std::exception &ex) {
        std::cerr << "Error in session: " << ex.what() << std::endl;
        return;
    }
}

void Server::chat_session(boost::asio::ip::tcp::socket &socket) {
    try {
        //boost::asio::ip::tcp::socket socket(io_context);
        //acceptor.accept(socket);

        boost::beast::flat_buffer buffer;
        std::fstream server_chat_file;
        std::mutex mutex_server_file;

        std::cout << "\nConnection from: " << (socket).remote_endpoint().address().to_string() << std::endl;
        std::cout << "Current IP: " << (socket).local_endpoint().address().to_string() << std::endl;

        server_chat_file.open((socket).local_endpoint().address().to_string() + "_server_chat.txt",
                              std::ios::in | std::ios::out | std::ios::app);

        if (!server_chat_file.is_open())
            throw std::runtime_error("Unable to open or create the file server_chat.txt");
        std::cout << "\nPrivate Chat:\n" << std::endl;

        boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws(std::move(socket));

        ws.binary(true);
        ws.accept(); // Handshake

        std::string message_get, message_send;
        server_chat_file.seekg(0, std::ios::beg);
        while (std::getline(server_chat_file, message_get)) {
            if (message_get.at(0) == ' ')
                std::cout << std::setw(28) << "(Received): " << message_get << std::endl;
            else
                std::cout << message_get << std::endl;
        }
        message_get.clear();
        std::cout << "\nLive Chat:\n" << std::endl;

        if (server_chat_file.fail())
            server_chat_file.clear();
        server_chat_file.seekp(0, std::ios::end);

        std::thread data_client_thread = std::thread(
            &Server::listen_data_client, this, &ws, &buffer, &server_chat_file, &mutex_server_file, &message_send);
        //std::thread data_client_thread = std::thread(std::bind(&Server::listen_data_client, this, &ws));
        //std::thread data_client_thread = std::thread([this, &ws] { this->listen_data_client(&ws); });

        std::cout << "\nAfter Live Chat:\n" << std::endl;

        while (true) {
            std::getline(std::cin, message_send);

            for (const char c : message_send) {
                if (!std::iswspace(c)) {
                    size_t space_pos = message_send.find(c);
                    if (std::string::npos == space_pos)
                        throw std::logic_error("Impossible to cut spaces");
                    message_send = message_send.substr(space_pos);

                    std::unique_lock<std::mutex> lock(mutex_server_file);
                    if (ws.is_open())
                        ws.write(boost::asio::buffer(message_send));
                    else
                        break;
                    if (message_send == "1111")
                        break;

                    server_chat_file << message_send << std::endl;
                    break;
                }
            }
            if (!ws.is_open() || message_send == "1111")
                break;
        }

        if (data_client_thread.joinable())
            data_client_thread.join();
        
        boost::beast::websocket::close_reason cr;
        cr.reason = "Goodbye"; // close reason
        ws.close(cr);

        server_chat_file.close();

        std::cout << "\n***** GOODBYE SERVER *****" << std::endl;
    }
    catch (const std::exception &ex) {
        std::cerr << "Error in session: " << ex.what() << std::endl;
        return;
    }
}

void Server::listen_data_client(boost::beast::websocket::stream<boost::asio::ip::tcp::socket> *ws,
                                boost::beast::flat_buffer *buffer, std::fstream *server_chat_file,
                                std::mutex *mutex_server_file, const std::string *message_send) {
    try {
        if (!(*server_chat_file).is_open())
            throw std::runtime_error("Unable to open or create the file server_chat.txt");
        std::string buffer_data;
        while (true) {
            (*buffer).consume((*buffer).size());
            if ((*ws).read(*buffer) > 0) {
                buffer_data = boost::beast::buffers_to_string((*buffer).data());
                if (buffer_data == "1111")
                    return;
                std::unique_lock<std::mutex> lock(*mutex_server_file);
                std::cout << std::setw(28) << "(Received): " << buffer_data << std::endl;
                (*server_chat_file) << " " << buffer_data << std::endl;
            }
            if ((*message_send) == "1111")
                return;
        }
    }
    catch (const std::exception &ex) {
        std::cerr << "listen data client error: " << ex.what() << std::endl;
        return;
    }
}
