#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_SERVER_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_SERVER_H

#include <memory>
#include <thread>
#include <vector>

namespace server_detail {
class Impl;
}

class Server {
 public:
  explicit Server(int concurrency);

  ~Server();

  void run(const char *address, unsigned short port);

 private:
  std::unique_ptr<server_detail::Impl> impl;
  int concurrency;
  std::vector<std::thread> threads;
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_SERVER_H
