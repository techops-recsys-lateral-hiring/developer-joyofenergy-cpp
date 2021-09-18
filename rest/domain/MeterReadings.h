#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGS_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGS_H

#include <domain/ElectricityReading.h>

#include <list>
#include <string>

class MeterReadings {
 public:
  MeterReadings() {}

  MeterReadings(std::string smartMeterId, std::list<ElectricityReading> electricityReadings)
      : smartMeterId(smartMeterId), electricityReadings(electricityReadings){};

  std::list<ElectricityReading> getElectricityReadings() { return electricityReadings; }

  std::string getSmartMeterId() { return smartMeterId; }

 private:
  std::list<ElectricityReading> electricityReadings;
  std::string smartMeterId;
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGS_H
