# websocket_boost_test
The provided code is for a basic client-server chat application using WebSockets in C++ with the Boost.Asio and Boost.Beast libraries.

This application facilitates communication between clients and a server via WebSockets.
- `main.cpp`: The entry point of the application that initializes either the server or client based on user input.
- `client.cpp` and `client.hpp`: Define the client's behavior, including connecting to the server and handling chat session.
- `server.cpp` and `server.hpp`: Define the server's behavior, including accepting connections and handling chat session.

**Initialization (`main.cpp`)**:
The application sets the console code page to UTF-8 using `system("chcp 65001");` for proper character encoding.
It prompts the user to enter a value to determine whether to start a server or a client.

**Server Mode**:
When the user enters `1`, a server is started.
The `Server` object initializes a TCP acceptor to listen on port 4444.
It enters a loop to accept client connections and handle them in separate threads using the `chat_session` method.

**Client Mode**:
When the user enters `2`, `3`, or `4`, a client is started, connecting to a specified server IP (entered by the user or hardcoded).
The `Client` object connects to the server on port 4444 and starts a chat session.

### Detailed Functionality

**Client (`client.cpp`)**:
   - **connect_socket**: Connects to the server and establishes a WebSocket connection.
   - **chat_session**: Handles chat by sending messages to the server, saving them to a file, and starting a thread to listen for messages from the server.
   - **listen_data_server**: Continuously listens for messages from the server and writes them to a file.

**Server (`server.cpp`)**:
   - **init_acceptor**: Initializes the TCP acceptor to listen for incoming connections.
   - **listen_server**: Accepts connections and starts new threads for each client using `chat_session`.
   - **chat_session**: Handles chat by receiving messages from the client, writing them to a file, and starting a thread to listen for messages from the client.
   - **listen_data_client**: Continuously listens for messages from the client and writes them to a file.

### Key Libraries and Techniques

1. **Boost.Asio**: Provides the underlying network functionality for TCP connections.
2. **Boost.Beast**: Handles WebSocket communication.
3. **Multithreading**: The server uses multithreading to handle multiple client connections simultaneously.
4. **File I/O**: Messages are logged to files for both clients and servers to maintain a chat history.
5. **Synchronization**: Mutexes are used to ensure thread-safe access to files.

This chat application is designed for simplicity and demonstrates fundamental concepts of network programming, WebSockets, and concurrency in C++. It allows multiple clients to connect to a server and communicate through a text-based chat system, with each session being logged to a file for persistence.
