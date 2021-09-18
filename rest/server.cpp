#include "server.h"

#include <controller/MeterReadingController.h>
#include <controller/PricePlanComparatorController.h>

#include "configuration.h"
#include "listener.h"
#include "router.h"

namespace server_detail {
class impl {
 public:
  explicit impl(int concurrency) : ioc(concurrency) {
    using reading = MeterReadingController;
    using price_plan = PricePlanComparatorController;
    router.to<reading, &reading::Read>(R"(/readings/read/([a-zA-Z0-9_-]+))", electricityReadingService, meterReadingService);
    router.to<reading, &reading::Store>(R"(/readings/store)", electricityReadingService, meterReadingService);
    router.to<price_plan, &price_plan::Compare>(R"(/price-plans/compare-all/([a-zA-Z0-9_-]+))", pricePlanService);
    router.to<price_plan, &price_plan::Recommend>(R"(/price-plans/recommend/([a-zA-Z0-9_-]+)\?(limit)=([0-9]+))",
                                                  pricePlanService);
  }

  void launch(const char *address, unsigned short port) {
    using tcp = boost::asio::ip::tcp;
    auto endpoint = tcp::endpoint{boost::asio::ip::make_address(address), port};
    std::make_shared<listener>(ioc, endpoint, handler)->run();
  }

  void run() { ioc.run(); }

  void stop() { ioc.stop(); }

 private:
  boost::asio::io_context ioc;
  std::unordered_map<std::string, std::vector<ElectricityReading>> meterAssociatedReadings{readings()};
  ElectricityReadingService electricityReadingService{meterAssociatedReadings};
  MeterReadingService meterReadingService{meterAssociatedReadings};
  std::vector<PricePlan> price_plans{pricePlans()};
  PricePlanService pricePlanService{price_plans, meterReadingService};
  router router;
  std::function<http::response<http::string_body>(const http::request<http::string_body> &)> handler = router.handler();
};
}  // namespace server_detail

server::server(int concurrency) : impl(std::make_unique<server_detail::impl>(concurrency)), concurrency(concurrency) {}

server::~server() {
  impl->stop();
  for (auto &worker : threads) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

void server::run(const char *address, unsigned short port) {
  impl->launch(address, port);
  threads.reserve(concurrency);
  for (auto i = concurrency; i > 0; --i) {
    threads.emplace_back([this] { impl->run(); });
  }
}
