#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_LISTENER_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_LISTENER_H

#include <boost/asio/strand.hpp>

#include "session.h"

class Listener : public std::enable_shared_from_this<Listener> {
  using io_context = boost::asio::io_context;
  using tcp = boost::asio::ip::tcp;
  using error_code = boost::beast::error_code;

 public:
  Listener(io_context &ioc, tcp::endpoint endpoint,
           std::function<http::response<http::string_body>(const http::request<http::string_body> &)> &handler)
      : ioc_(ioc), acceptor_(boost::asio::make_strand(ioc)), handler(handler) {
    error_code ec;

    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
      fail(ec, "open");
      return;
    }

    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec) {
      fail(ec, "set_option");
      return;
    }

    acceptor_.bind(endpoint, ec);
    if (ec) {
      fail(ec, "bind");
      return;
    }

    acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
    if (ec) {
      fail(ec, "listen");
      return;
    }
  }

  void run() { do_accept(); }

 private:
  void do_accept() {
    acceptor_.async_accept(boost::asio::make_strand(ioc_),
                           boost::beast::bind_front_handler(&Listener::on_accept, shared_from_this()));
  }

  void on_accept(error_code ec, tcp::socket socket) {
    if (ec) {
      fail(ec, "accept");
    } else {
      std::make_shared<Session>(std::move(socket), handler)->run();
    }
    do_accept();
  }

  io_context &ioc_;
  tcp::acceptor acceptor_;
  std::function<http::response<http::string_body>(const http::request<http::string_body> &)> &handler;
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_LISTENER_H
