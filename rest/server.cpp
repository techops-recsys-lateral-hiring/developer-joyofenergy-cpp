#include "server.h"

#include <controller/MeterReadingController.h>
#include <controller/PricePlanComparatorController.h>

#include "configuration.h"
#include "listener.h"
#include "router.h"

namespace server_detail {
class impl {
 public:
  explicit impl(int concurrency) : ioc_(concurrency) {
    using reading = MeterReadingController;
    using price_plan = PricePlanComparatorController;
    router_.to<reading, &reading::Read>(R"(/readings/read/([a-zA-Z0-9_-]+))", electricityReadingService_, meterReadingService_);
    router_.to<reading, &reading::Store>(R"(/readings/store)", electricityReadingService_, meterReadingService_);
    router_.to<price_plan, &price_plan::Compare>(R"(/price-plans/compare-all/([a-zA-Z0-9_-]+))", pricePlanService_);
    router_.to<price_plan, &price_plan::Recommend>(R"(/price-plans/recommend/([a-zA-Z0-9_-]+)\?(limit)=([0-9]+))",
                                                   pricePlanService_);
  }

  void launch(const char *address, unsigned short port) {
    using tcp = boost::asio::ip::tcp;
    auto endpoint = tcp::endpoint{boost::asio::ip::make_address(address), port};
    std::make_shared<listener>(ioc_, endpoint, handler_)->run();
  }

  void run() { ioc_.run(); }

  void stop() { ioc_.stop(); }

 private:
  boost::asio::io_context ioc_;
  std::unordered_map<std::string, std::vector<ElectricityReading>> meterAssociatedReadings_{readings()};
  ElectricityReadingService electricityReadingService_{meterAssociatedReadings_};
  MeterReadingService meterReadingService_{meterAssociatedReadings_};
  std::vector<PricePlan> price_plans_{pricePlans()};
  PricePlanService pricePlanService_{price_plans_, meterReadingService_};
  router router_;
  std::function<http::response<http::string_body>(const http::request<http::string_body> &)> handler_ = router_.handler();
};
}  // namespace server_detail

server::server(int concurrency) : impl_(std::make_unique<server_detail::impl>(concurrency)), concurrency_(concurrency) {}

server::~server() {
  impl_->stop();
  for (auto &worker : threads_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

void server::run(const char *address, unsigned short port) {
  impl_->launch(address, port);
  threads_.reserve(concurrency_);
  for (auto i = concurrency_; i > 0; --i) {
    threads_.emplace_back([this] { impl_->run(); });
  }
}
