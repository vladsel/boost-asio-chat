#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <fstream>
#include <string>
#include <mutex>

class Client {
public:
	explicit Client() : io_context(), ws(io_context) {}
	~Client() { ws.close(boost::beast::websocket::close_code::normal); 
				io_context.stop(); client_chat_file.close(); }
	void connect_socket(const std::string client_ip, const boost::asio::ip::port_type port);
	void echo_session();
	void chat_session();
private:
	void listen_data_server();
	boost::asio::io_context io_context;
	boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws;
	boost::beast::flat_buffer buffer;
	std::fstream client_chat_file;
	std::mutex mutex_chat_file;
	std::string message_get, message_send;
};
