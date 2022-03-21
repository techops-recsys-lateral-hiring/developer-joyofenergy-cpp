#include <iostream>

#include "rest/server.h"

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: app <address> <kPort> <concurrency>\n"
              << "Example:\n"
              << "    http-server-async 0.0.0.0 8080 1\n";
    return EXIT_FAILURE;
  }
  auto const address = argv[1];
  auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
  auto const threads = std::max<int>(1, std::atoi(argv[3]));
  Server server{threads};
  server.run(address, port);
  char wait;
  std::cin >> wait;
  return EXIT_SUCCESS;
}