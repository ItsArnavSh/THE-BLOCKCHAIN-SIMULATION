#include <iostream>
#include <boost/asio.hpp>
#include "nlohmann/json.hpp"
using boost::asio::ip::tcp;
using json = nlohmann::json;

void start_server(unsigned short port);
