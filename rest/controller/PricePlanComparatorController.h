#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANCOMPARATORCONTROLLER_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANCOMPARATORCONTROLLER_H

#include <service/ElectricityReadingService.h>

#include <ctime>
#include <iomanip>

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

#include <service/PricePlanService.h>
#include <iostream>

namespace http = boost::beast::http;

namespace price_detail {
    auto renderCostsAsJson(const ElectricityReading &r) {
        return nlohmann::json{{"hehe"},
                              {"reading"}};
        }
    }  // namespace price_detail

    class PricePlanComparatorController {
    public:
        PricePlanComparatorController(PricePlanService &pricePlanService) : pricePlanService(pricePlanService) {}

//        http::response<http::string_body>
//        Read(const http::request<http::string_body> &req, const std::vector<std::string> &queries) {
//            const auto &meterId = queries[0];
//            auto costs = pricePlanService.getConsumptionCostOfElectricityReadingsForEachPricePlan(meterId);
//            for(auto &i:*costs){
//                std::cout<<i.first<<" "<<i.second<<std::endl;
//            }
//
//            if (!costs) {
//                return {http::status::not_found, req.version()};
//            }
//            http::response<http::string_body> res{http::status::ok, req.version()};
//            res.set(http::field::content_type, "application/json");
//            res.keep_alive(req.keep_alive());
//            auto results = nlohmann::json::array();
//            std::transform(costs->begin(), costs->end(), std::back_inserter(results), &price_detail::renderCostsAsJson);
//            nlohmann::json j;
//            j["readings"] = results;
//            res.body() = j.dump();
//            res.prepare_payload();
//            return res;
//        }

    private:
        PricePlanService &pricePlanService;
    };

#endif //DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANCOMPARATORCONTROLLER_H
