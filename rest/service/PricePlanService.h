#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANSERVICE_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANSERVICE_H

#include <configuration.h>
#include <controller/MeterReadingController.h>
#include <domain/ElectricityReading.h>
#include <domain/PricePlan.h>
#include <service/MeterReadingService.h>

#include <ctime>
#include <map>
#include <optional>
#include <string>
#include <vector>

class PricePlanService {
 public:
  using time_point_type = std::chrono::time_point<std::chrono::system_clock>;

  std::optional<std::map<std::string, int>> getConsumptionCostOfElectricityReadingsForEachPricePlan(std::string smartMeterId) {
    std::optional<std::vector<ElectricityReading>> electricityReadings = meterReadingService.getReadings(smartMeterId);
    if (!electricityReadings.has_value()) {
      return {};
    }

    std::map<std::string, int> consumptionCostOfElectricityReadingsForEachPricePlan;
    for (auto pricePlan : pricePlans) {
      consumptionCostOfElectricityReadingsForEachPricePlan.insert(
          std::make_pair(pricePlan.getPlanName(), calculateCost(electricityReadings.value(), pricePlan)));
    }
    return consumptionCostOfElectricityReadingsForEachPricePlan;
  }

  PricePlanService(std::vector<PricePlan> &pricePlans, MeterReadingService &meterReadingService)
      : pricePlans(pricePlans), meterReadingService(meterReadingService) {}

 private:
  const std::vector<PricePlan> &pricePlans;
  MeterReadingService &meterReadingService;

  static auto calculateTimeElapsed(std::vector<ElectricityReading> electricityReadings) {
    ElectricityReading first = *electricityReadings.begin();
    ElectricityReading last = *electricityReadings.begin();
    std::vector<ElectricityReading>::iterator it;
    for (it = electricityReadings.begin(); it != electricityReadings.end(); it++) {
      if (it->getTime() < first.getTime()) {
        first = *it;
      }
      if (it->getTime() > first.getTime()) {
        last = *it;
      }
    }

    std::chrono::duration duration = last.getTime() - first.getTime();

    return duration;
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
