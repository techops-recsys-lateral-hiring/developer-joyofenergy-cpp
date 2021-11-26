#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLAN_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLAN_H

#include <string>
#include <utility>
#include <vector>
#include <chrono>

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
      : planName_(std::move(planName)),
        energySupplier_(std::move(energySupplier)),
        unitRate_(unitRate),
        peakTimeMultipliers_(std::move(peakTimeMultipliers)) {}

  std::string getEnergySupplier() const { return energySupplier_; }

  std::string getPlanName() const { return planName_; }

  int getUnitRate() const { return unitRate_; }

  int getPrice(time_point_type dateTime) const {
    auto time_t_dateTime = std::chrono::system_clock::to_time_t(dateTime);
    auto t = std::localtime(&time_t_dateTime);
    auto it = std::find_if(peakTimeMultipliers_.begin(), peakTimeMultipliers_.end(),
                           [=](auto &p) { return p.dayOfWeek == t->tm_wday; });
    if (it == peakTimeMultipliers_.end()) {
      return unitRate_;
    }
    return unitRate_ * it->multiplier;
  }

 private:
  const std::string energySupplier_;
  const std::string planName_;
  const int unitRate_;  // unit price per kWh
  const std::vector<PeakTimeMultiplier> peakTimeMultipliers_;
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLAN_H
