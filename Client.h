#include <iostream>
#include <boost/asio.hpp>
#include "nlohmann/json.hpp"

using boost::asio::ip::tcp;
using json = nlohmann::json;

void send_json(const std::string& host, unsigned short port);
