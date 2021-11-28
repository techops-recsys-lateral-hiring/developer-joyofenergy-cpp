#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANSERVICE_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANSERVICE_H

#include <configuration.h>
#include <domain/ElectricityReading.h>
#include <domain/PricePlan.h>
#include <service/MeterReadingService.h>

#include <algorithm>
#include <ctime>
#include <map>
#include <optional>
#include <string>
#include <vector>

class PricePlanService {
 public:
  using time_point_type = std::chrono::time_point<std::chrono::system_clock>;

  std::optional<std::map<std::string, int>> getConsumptionCostOfElectricityReadingsForEachPricePlan(const std::string &smartMeterId) const {
    std::optional<std::vector<ElectricityReading>> electricityReadings = meterReadingService_.getReadings(smartMeterId);
    if (!electricityReadings.has_value()) {
      return {};
    }

    std::map<std::string, int> consumptionCostOfElectricityReadingsForEachPricePlan;
    for (auto pricePlan : pricePlans_) {
      consumptionCostOfElectricityReadingsForEachPricePlan.insert(
          std::make_pair(pricePlan.getPlanName(), calculateCost(electricityReadings.value(), pricePlan)));
    }
    return consumptionCostOfElectricityReadingsForEachPricePlan;
  }

  PricePlanService(std::vector<PricePlan> &pricePlans, MeterReadingService &meterReadingService)
      : pricePlans_(pricePlans), meterReadingService_(meterReadingService) {}

 private:
  const std::vector<PricePlan> &pricePlans_;
  MeterReadingService &meterReadingService_;

  static auto calculateTimeElapsed(const std::vector<ElectricityReading> &electricityReadings) {
    const auto [min, max] = std::minmax_element(std::begin(electricityReadings), std::end(electricityReadings),
                                                [](auto const &l, auto const &r) { return l.getTime() < r.getTime(); });

    return max->getTime() - min->getTime();
  }

  static int calculateCost(const std::vector<ElectricityReading> &electricityReadings, const PricePlan &pricePlan) {
    int average = calculateAverageReading(electricityReadings);
    auto secondsElapsed = std::chrono::duration_cast<std::chrono::seconds>(calculateTimeElapsed(electricityReadings));

    int averagedCost = average * 3600 / secondsElapsed.count();
    return averagedCost * pricePlan.getUnitRate();
  }

  static int calculateAverageReading(const std::vector<ElectricityReading> &electricityReadings) {
    int summedReadings = 0;
    for (auto &electricityReading : electricityReadings) {
      summedReadings += electricityReading.getReading();
    }

    return summedReadings / int(electricityReadings.size());
  }
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANSERVICE_H
