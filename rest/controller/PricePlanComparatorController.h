#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANCOMPARATORCONTROLLER_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANCOMPARATORCONTROLLER_H

#include <configuration.h>
#include <service/ElectricityReadingService.h>
#include <service/PricePlanService.h>

#include <ctime>
#include <iomanip>
#include <iostream>

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;

class PricePlanComparatorController {
 public:
  PricePlanComparatorController(PricePlanService &pricePlanService) : pricePlanService(pricePlanService) {}

  http::response<http::string_body> Compare(const http::request<http::string_body> &req,
                                            const std::vector<std::string> &queries) {
    const auto &meterId = queries[0];
    auto costs = pricePlanService.getConsumptionCostOfElectricityReadingsForEachPricePlan(meterId);

    if (!costs) {
      return {http::status::not_found, req.version()};
    }
    auto current_price_plans = smart_meter_to_price_plan_accounts();
    http::response<http::string_body> res{http::status::ok, req.version()};
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req.keep_alive());
    nlohmann::json j;
    for (auto &ele : costs.value()) {
      ele.second /= 10000;
    }
    j["pricePlanComparisons"] = costs.value();
    j["pricePlanId"] = current_price_plans[meterId];
    res.body() = j.dump();
    res.prepare_payload();
    return res;
  }

  http::response<http::string_body> Recommend(const http::request<http::string_body> &req,
                                              const std::vector<std::string> &queries) {
    const auto &meterId = queries[0];
    std::optional<int> maybeLimit;
    if (queries.size() > 2){
      maybeLimit = std::stoi(queries[2]);
    }
    auto costs = pricePlanService.getConsumptionCostOfElectricityReadingsForEachPricePlan(meterId);

    if (!costs) {
      return {http::status::not_found, req.version()};
    }

    std::vector<std::pair<std::string, double>> ordered_costs{costs->begin(), costs->end()};
    std::sort(ordered_costs.begin(), ordered_costs.end(),
              [](auto &cost_a, auto &cost_b) { return cost_a.second < cost_b.second; });
    if (maybeLimit.has_value()) {
      ordered_costs.resize(std::min(maybeLimit.value(), int(ordered_costs.size())));
    }

    http::response<http::string_body> res{http::status::ok, req.version()};
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req.keep_alive());
    auto results = nlohmann::json::array();
    std::transform(ordered_costs.begin(), ordered_costs.end(), std::back_inserter(results), [](auto &cost) {
      return nlohmann::json{{cost.first, double(cost.second) / 10000}};
    });
    nlohmann::json j;
    j["recommend"] = results;
    res.body() = j.dump();
    res.prepare_payload();
    return res;
  }

 private:
  PricePlanService &pricePlanService;
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANCOMPARATORCONTROLLER_H
