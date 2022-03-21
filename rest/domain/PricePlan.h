#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLAN_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLAN_H

#include <chrono>
#include <string>
#include <utility>
#include <vector>

class PricePlan {
  // todo:
 public:
  using time_point_type = std::chrono::time_point<std::chrono::system_clock>;
  class PeakTimeMultiplier {
   public:
    enum DayOfWeek {
      SUNDAY,
      MONDAY,
      TUESDAY,
      WEDNESDAY,
      THURSDAY,
      FRIDAY,
      SATURDAY
    };

    PeakTimeMultiplier(DayOfWeek dayOfWeek, int multiplier) : dayOfWeek(dayOfWeek), multiplier(multiplier) {}

    DayOfWeek dayOfWeek;
    int multiplier;
  };

  PricePlan(std::string planName, std::string energySupplier, int unitRate, std::vector<PeakTimeMultiplier> peakTimeMultipliers)
      : planName(std::move(planName)), energySupplier(std::move(energySupplier)), unitRate(unitRate), peakTimeMultipliers(std::move(peakTimeMultipliers)) {}

  std::string getEnergySupplier() const { return energySupplier; }

  std::string getPlanName() const { return planName; }

  int getUnitRate() const { return unitRate; }

  int getPrice(time_point_type dateTime) const {
    auto time_t_dateTime = std::chrono::system_clock::to_time_t(dateTime);
    auto t = std::localtime(&time_t_dateTime);
    auto it = std::find_if(peakTimeMultipliers.begin(), peakTimeMultipliers.end(), [=](auto &p) {
      return p.dayOfWeek == t->tm_wday;
    });
    if (it == peakTimeMultipliers.end()) {
      return unitRate;
    }
    return unitRate * it->multiplier;
  }

 private:
  const std::string energySupplier;
  const std::string planName;
  const int unitRate;  // unit price per kWh
  const std::vector<PeakTimeMultiplier> peakTimeMultipliers;
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLAN_H
