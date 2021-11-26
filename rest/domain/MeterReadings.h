#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGS_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGS_H

#include <domain/ElectricityReading.h>

#include <list>
#include <string>

class MeterReadings {
 public:
  MeterReadings() {}

  MeterReadings(std::string smartMeterId, std::list<ElectricityReading> electricityReadings)
      : smartMeterId_(smartMeterId), electricityReadings_(electricityReadings){};

  std::list<ElectricityReading> getElectricityReadings() { return electricityReadings_; }

  std::string getSmartMeterId() { return smartMeterId_; }

 private:
  std::list<ElectricityReading> electricityReadings_;
  std::string smartMeterId_;
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGS_H
