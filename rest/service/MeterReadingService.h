//
// Created by Cenxi Zhang on 2021/8/18.
//

#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGSERVICE_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGSERVICE_H

#include <domain/ElectricityReading.h>

#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

class MeterReadingService {
 public:
  std::optional<std::vector<ElectricityReading>> getReadings(const std::string &smartMeterId) {
    if (meterAssociatedReadings.find(smartMeterId) == meterAssociatedReadings.end()) {
      return {};
    }
    return meterAssociatedReadings[smartMeterId];
  }

  void storeReadings(const std::string &smartMeterId, std::vector<ElectricityReading> &electricityReadings) {
    if (meterAssociatedReadings.find(smartMeterId) == meterAssociatedReadings.end()) {
      meterAssociatedReadings[smartMeterId] = {};
    }
    meterAssociatedReadings[smartMeterId].insert(meterAssociatedReadings[smartMeterId].end(), electricityReadings.begin(),
                                                 electricityReadings.end());
  }

  explicit MeterReadingService(std::unordered_map<std::string, std::vector<ElectricityReading>> &meterAssociatedReadings)
      : meterAssociatedReadings(meterAssociatedReadings) {}

 private:
  std::unordered_map<std::string, std::vector<ElectricityReading>> &meterAssociatedReadings;
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGSERVICE_H
