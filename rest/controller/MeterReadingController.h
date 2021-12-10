#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGCONTROLLER_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGCONTROLLER_H

#include <date/date.h>
#include <service/ElectricityReadingService.h>
#include <service/MeterReadingService.h>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;

namespace detail {
auto toRfc3339(std::chrono::time_point<std::chrono::system_clock> time) {
  return date::format("%FT%TZ", time);
}

auto fromRfc3339(const std::string &time) {
  std::chrono::time_point<std::chrono::system_clock> tp;
  std::istringstream ss(time);
  ss >> date::parse("%FT%TZ", tp);
  return tp;
}

auto renderReadingAsJson(const ElectricityReading &r) {
  return nlohmann::json{{"time", toRfc3339(r.getTime())}, {"reading", double(r.getReading()) / 10000}};
}
}  // namespace detail

class MeterReadingController {
 public:
  MeterReadingController(ElectricityReadingService &electricityReadingService, MeterReadingService &meterReadingService)
      : electricityReadingService_(electricityReadingService), meterReadingService_(meterReadingService) {}

  http::response<http::string_body> Read(const http::request<http::string_body> &req, const std::vector<std::string> &queries) const {
    const auto &meterId = queries[0];
    auto readings = electricityReadingService_.GetReading(meterId);

    if (!readings) {
      return {http::status::not_found, req.version()};
    }
    http::response<http::string_body> res{http::status::ok, req.version()};
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req.keep_alive());
    auto results = nlohmann::json::array();
    std::transform(readings->begin(), readings->end(), std::back_inserter(results), &detail::renderReadingAsJson);
    nlohmann::json j;
    j["readings"] = results;
    res.body() = j.dump();
    res.prepare_payload();
    return res;
  }

  http::response<http::string_body> Store(const http::request<http::string_body> &req,
                                          const std::vector<std::string> &queries) {
    auto body = nlohmann::json::parse(req.body());
    auto smartMeterId = body["smartMeterId"];
    std::vector<ElectricityReading> electricityReadings;
    if (!IsMeterReadingsValid(smartMeterId, body["electricityReadings"])) {
      return {http::status::internal_server_error, 11};
    }
    for (auto &electricityReading : body["electricityReadings"]) {
      electricityReadings.emplace_back(detail::fromRfc3339(electricityReading["time"]), electricityReading["reading"]);
    }
    meterReadingService_.storeReadings(smartMeterId, electricityReadings);
    return {};
  }

 private:
  ElectricityReadingService &electricityReadingService_;
  MeterReadingService &meterReadingService_;
  bool IsMeterReadingsValid(const nlohmann::basic_json<> &smartMeterId,
                            const nlohmann::basic_json<> &electricityReadings) const {
    if (smartMeterId.type() == nlohmann::json::value_t::null || electricityReadings.empty()) {
      return false;
    }
    return true;
  }
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGCONTROLLER_H
