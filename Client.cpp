#include "Client.h"
#include "Transaction.h"
#include "Server.h"
#include "wallet.h"
#include <string>
#include <thread>

// List of predefined IPs to send transactions to
std::vector<std::string> server_ips = {"127.0.0.1", "192.168.246.31", "192.168.246.29"};
const vector<unsigned short> port = {8080,8080,8080};

void send_json(const std::string& host, unsigned short port, const json& data) {
    try {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve(host, std::to_string(port)));

        std::string json_str = data.dump();
        boost::asio::write(socket, boost::asio::buffer(json_str));

        std::cout << "[📤] JSON sent to " << host << ":" << port << std::endl;
    } catch (std::exception& e) {
        std::cerr << "[❌] Exception: " << e.what() << " while sending to " << host << std::endl;
    }
}

void input() {
    Wallet w1;
    while (true) {
        cout << "Back Here"<<endl;
        int amt;
        std::cout << "Enter amount: "<<endl;
        std::cin >> amt;

        // Check if input failed (e.g., user entered a non-numeric value)
        if (std::cin.fail()) {
            std::cin.clear();  // Reset error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "[❌] Invalid input. Please enter a valid number.\n";
            continue; // Restart loop
        }

        std::cin.ignore();  // Clear the newline character from input buffer
        std::string rec;
        std::cout << "Enter receiver: "<<endl;
        std::getline(std::cin, rec);  // Use getline to avoid input issues

        std::cout << "Processing transaction...\n";
        Transaction t1(w1.public_key, rec, amt);
        t1.save_to_file();

        // Send transaction to all other nodes except itself
        for (size_t i = 0; i < server_ips.size(); ++i) {
            //if (server_ips[i] == "127.0.0.1" && port[i] == port[0]) continue;  // Skip self
            send_json(server_ips[i], port[i], t1.to_JSON());
            std::cout << "Hereeee" << std::endl; // Ensure output is flushed
        }
    }
}


int main() {
    unsigned short server_port = port[0];  // Use the first port for the server

    // Start server in a separate thread
    std::thread server_thread(start_server, server_port);

    // Run input() in the main thread
    input();

    // Join server thread (though input() runs indefinitely)
    server_thread.join();

    return 0;
}
