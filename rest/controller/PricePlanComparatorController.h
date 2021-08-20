#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANCOMPARATORCONTROLLER_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANCOMPARATORCONTROLLER_H

#include <service/ElectricityReadingService.h>
#include <service/PricePlanService.h>
#include <configuration.h>

#include <ctime>
#include <iomanip>

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>


#include <iostream>

namespace http = boost::beast::http;

class PricePlanComparatorController {
public:
    PricePlanComparatorController(PricePlanService &pricePlanService) : pricePlanService(pricePlanService) {}

    http::response<http::string_body>
    Compare(const http::request<http::string_body> &req, const std::vector<std::string> &queries) {
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
        j["pricePlanComparisons"] = {{"price-plan-0", double(costs.value()["price-plan-0"])/10000},
                                     {"price-plan-1", double(costs.value()["price-plan-1"])/10000},
                                     {"price-plan-2", double(costs.value()["price-plan-2"])/10000}};
        j["pricePlanId"] = current_price_plans[meterId];
        res.body() = j.dump();
        res.prepare_payload();
        return res;
    }

    http::response<http::string_body>
    Recommend(const http::request<http::string_body> &req, const std::vector<std::string> &queries) {
        const auto &meterId = queries[0];
        int limit = std::stoi(queries[2]);
        auto costs = pricePlanService.getConsumptionCostOfElectricityReadingsForEachPricePlan(meterId);

        if (!costs) {
            return {http::status::not_found, req.version()};
        }

        std::vector<std::pair<std::string, float>> ordered_costs{costs->begin(), costs->end()};
        std::sort(ordered_costs.begin(), ordered_costs.end(), [](auto &cost_a, auto &cost_b) {
            return cost_a.second < cost_b.second;
        });
        ordered_costs.resize(std::min(limit, int(ordered_costs.size())));

        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        auto results = nlohmann::json::array();
        std::transform(ordered_costs.begin(), ordered_costs.end(), std::back_inserter(results),
                       [](auto &cost) { return nlohmann::json{{cost.first, cost.second}}; });
        nlohmann::json j;
        j["recommend"] = results;
        res.body() = j.dump();
        res.prepare_payload();
        return res;
    }

private:
    PricePlanService &pricePlanService;
};

#endif //DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANCOMPARATORCONTROLLER_H
