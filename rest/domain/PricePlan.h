#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLAN_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLAN_H

#include <string>
#include <vector>

class PricePlan {
  // todo:
 public:
  class PeakTimeMultiplier {
    enum DayOfWeek {
      MONDAY,
      TUESDAY,
    };

    DayOfWeek dayOfWeek;
    int multiplier;

    PeakTimeMultiplier(DayOfWeek dayOfWeek, int multiplier) : dayOfWeek(dayOfWeek), multiplier(multiplier) {}
  };

  PricePlan(std::string planName, std::string energySupplier, int unitRate, std::vector<PeakTimeMultiplier> peakTimeMultipliers)
      : planName(planName), energySupplier(energySupplier), unitRate(unitRate), peakTimeMultipliers(peakTimeMultipliers) {}

  std::string getEnergySupplier() const { return energySupplier; }

  std::string getPlanName() const { return planName; }

  int getUnitRate() const { return unitRate; }

 private:
  const std::string energySupplier;
  const std::string planName;
  const int unitRate;  // unit price per kWh
  const std::vector<PeakTimeMultiplier> peakTimeMultipliers;
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLAN_H
