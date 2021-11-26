#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_RESTCLIENT_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_RESTCLIENT_H

#include <boost/beast.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class RestClient {
 public:
  RestClient(const char *host, unsigned short port) : host_(host), port_(std::to_string(port)) {}

  ~RestClient() {
    beast::error_code ec;
    stream_.socket().shutdown(tcp::socket::shutdown_both, ec);
  }

  http::response<http::string_body> Do(http::request<http::string_body> &req) {
    auto const endpoints = resolver_.resolve(host_, port_);
    stream_.connect(endpoints);
    req.set(http::field::host, host_);

    http::write(stream_, req);

    beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(stream_, buffer, res);
    return res;
  }

  http::response<http::string_body> Get(const char *path) {
    http::request<http::string_body> req{http::verb::get, path, 11};
    return Do(req);
  }

  http::response<http::string_body> Post(const char *path, const nlohmann::json &body) {
    http::request<http::string_body> req{http::verb::post, path, 11};
    req.set(http::field::content_type, "application/json");
    req.body() = body.dump();
    req.prepare_payload();
    return Do(req);
  }

  net::io_context ioc_;
  tcp::resolver resolver_{ioc_};
  beast::tcp_stream stream_{ioc_};
  std::string host_;
  std::string port_;
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_RESTCLIENT_H
