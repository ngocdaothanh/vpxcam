#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

#include "client.h"

using boost::asio::ip::tcp;

static tcp::socket* s;

void tcp_connect(char* host, char* port) {
  boost::asio::io_service io_service;

  tcp::resolver resolver(io_service);
  tcp::resolver::query query(tcp::v4(), host, port);
  tcp::resolver::iterator iterator = resolver.resolve(query);

  s = new tcp::socket(io_service);
  boost::asio::connect(*s, iterator);
}

void tcp_read(char* buffer, int length) {
  boost::asio::read(*s, boost::asio::buffer(buffer, length));
}
