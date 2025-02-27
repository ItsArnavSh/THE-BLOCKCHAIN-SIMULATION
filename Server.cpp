#include "Server.h"
#include <fstream>

using boost::asio::ip::tcp;
using json = nlohmann::json;

void start_server(unsigned short port) {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

        std::ofstream log_file("server_log.json", std::ios::app); // Append mode
        if (!log_file) {
            std::cerr << "[❌] Failed to open log file" << std::endl;
            return;
        }

        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            char buffer[1024];
            boost::system::error_code error;
            size_t length = socket.read_some(boost::asio::buffer(buffer), error);

            if (error && error != boost::asio::error::eof) {
                continue;
            }

            std::string received_data(buffer, length);
            try {
                json received_json = json::parse(received_data);
                log_file << received_json.dump(4) << std::endl; // Write JSON to file
            } catch (json::parse_error&) {
                // Ignore malformed JSON
            }
        }
    } catch (std::exception&) {
        // Silent failure
    }
}
