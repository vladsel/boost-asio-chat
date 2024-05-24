#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <fstream>
#include <string>

class Server {
public:
	explicit Server() : io_context(), acceptor(io_context) {}
	~Server() { acceptor.close(), io_context.stop(); }
	void init_acceptor();
	void listen_server(void (Server:: *session)(boost::asio::ip::tcp::socket &socket));
	void echo_session();
	void chat_session(boost::asio::ip::tcp::socket &socket);
private:
	void listen_data_client(boost::beast::websocket::stream<boost::asio::ip::tcp::socket> *ws,
							boost::beast::flat_buffer *buffer, std::fstream *server_chat_file,
							std::mutex *mutex_server_file, const std::string *message_send);
	boost::asio::io_context io_context;
	boost::asio::ip::tcp::acceptor acceptor;
	//boost::beast::flat_buffer buffer;
	//std::fstream server_chat_file;
	//std::mutex mutex_server_file;
};
