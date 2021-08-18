#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_RESTCLIENT_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_RESTCLIENT_H

#include <boost/beast.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class RestClient {
public:
    RestClient(const char *host, unsigned short port) : host(host), port(std::to_string(port)) {}

    ~RestClient() {
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);
    }

    http::response<http::string_body> Do(http::request<http::string_body> &req) {
        auto const endpoints = resolver.resolve(host, port);
        stream.connect(endpoints);
        req.set(http::field::host, host);

        http::write(stream, req);

        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream, buffer, res);
        return res;
    }

    http::response<http::string_body> Get(const char *path) {
        http::request<http::string_body> req{http::verb::get, path, 11};
        return Do(req);
    }

    net::io_context ioc;
    tcp::resolver resolver{ioc};
    beast::tcp_stream stream{ioc};
    std::string host;
    std::string port;
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_RESTCLIENT_H
