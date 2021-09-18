#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_CONFIGURATION_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_CONFIGURATION_H

#include <domain/ElectricityReading.h>
#include <domain/PricePlan.h>

#include "generator.h"

constexpr auto DR_EVILS_DARK_ENERGY_ENERGY_SUPPLIER = "Dr Evil's Dark Energy";
constexpr auto THE_GREEN_ECO_ENERGY_SUPPLIER = "The Green Eco";
constexpr auto POWER_FOR_EVERYONE_ENERGY_SUPPLIER = "Power for Everyone";

constexpr auto MOST_EVIL_PRICE_PLAN_ID = "price-plan-0";
constexpr auto RENEWABLES_PRICE_PLAN_ID = "price-plan-1";
constexpr auto STANDARD_PRICE_PLAN_ID = "price-plan-2";

constexpr auto SARAHS_SMART_METER_ID = "smart-meter-0";
constexpr auto PETERS_SMART_METER_ID = "smart-meter-1";
constexpr auto CHARLIES_SMART_METER_ID = "smart-meter-2";
constexpr auto ANDREAS_SMART_METER_ID = "smart-meter-3";
constexpr auto ALEXS_SMART_METER_ID = "smart-meter-4";

std::vector<PricePlan> pricePlans() {
  std::vector<PricePlan> price_plans;
  price_plans.push_back(PricePlan(MOST_EVIL_PRICE_PLAN_ID, DR_EVILS_DARK_ENERGY_ENERGY_SUPPLIER, 10, {}));
  price_plans.push_back(PricePlan(RENEWABLES_PRICE_PLAN_ID, THE_GREEN_ECO_ENERGY_SUPPLIER, 2, {}));
  price_plans.push_back(PricePlan(STANDARD_PRICE_PLAN_ID, POWER_FOR_EVERYONE_ENERGY_SUPPLIER, 1, {}));
  return price_plans;
}

std::unordered_map<std::string, std::string> smart_meter_to_price_plan_accounts() {
  return {{SARAHS_SMART_METER_ID, MOST_EVIL_PRICE_PLAN_ID},
          {PETERS_SMART_METER_ID, RENEWABLES_PRICE_PLAN_ID},
          {CHARLIES_SMART_METER_ID, MOST_EVIL_PRICE_PLAN_ID},
          {ANDREAS_SMART_METER_ID, STANDARD_PRICE_PLAN_ID},
          {ALEXS_SMART_METER_ID, RENEWABLES_PRICE_PLAN_ID}};
};

auto readings() {
  std::unordered_map<std::string, std::vector<ElectricityReading>> result;
  for (auto &[meter, plan] : smart_meter_to_price_plan_accounts()) {
    result[meter] = generator{}.generate(21);
  }
  return result;
}

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_CONFIGURATION_H
