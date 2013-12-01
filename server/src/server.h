#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class server {
public:
  server(int width, int height, short port) {
    memcpy(resolution,     &width,  4);
    memcpy(resolution + 4, &height, 4);

    io_service = new boost::asio::io_service();
    acceptor   = new tcp::acceptor(*io_service, tcp::endpoint(tcp::v4(), port));
  }

  ~server() {
    delete io_service;
    delete acceptor;
  }

  void run() {
    start_accept();
    io_service->run();
  }

  void handle_encoded(char* encoded, int size) {
    mtx.lock();

    boost::shared_ptr<std::vector<char> > data(new std::vector<char>(encoded, encoded + size));
    char* b4 = (char*) (&size);
    data->insert(data->begin(), b4[3]);
    data->insert(data->begin(), b4[2]);
    data->insert(data->begin(), b4[1]);
    data->insert(data->begin(), b4[0]);

    std::vector<tcp::socket*>::iterator itr = clients.begin();
    while (itr != clients.end()) {
      tcp::socket* socket = *itr;
      boost::asio::async_write(
        *socket,
        boost::asio::buffer(*data),
        boost::bind(&server::handle_write_frame, this, socket, boost::asio::placeholders::error)
      );
      itr++;
    }

    mtx.unlock();
  }

private:
  void start_accept() {
    tcp::socket* socket = new tcp::socket(*io_service);
    acceptor->async_accept(
      *socket,
      boost::bind(&server::handle_accept, this, socket, boost::asio::placeholders::error)
    );
  }

  void handle_accept(tcp::socket* socket, const boost::system::error_code& error) {
    if (!error) {
      boost::asio::async_write(
        *socket,
        boost::asio::buffer(resolution, 2 * 4),
        boost::bind(&server::handle_write_resolution, this, socket, boost::asio::placeholders::error)
      );
    }

    start_accept();
  }

  void handle_write_resolution(tcp::socket* socket, const boost::system::error_code& error) {
    if (!error) {
      mtx.lock();
      clients.push_back(socket);
      mtx.unlock();
    }
  }

  void handle_write_frame(tcp::socket* socket, const boost::system::error_code& error) {
    if (error) {
      mtx.lock();
      clients.erase(std::find(clients.begin(), clients.end(), socket));
      mtx.unlock();
    }
  }

private:
  boost::asio::io_service* io_service;
  tcp::acceptor*           acceptor;

  char resolution[2 * 4];
  char cmd;

  std::vector<tcp::socket*> clients;
  boost::mutex mtx;
};
