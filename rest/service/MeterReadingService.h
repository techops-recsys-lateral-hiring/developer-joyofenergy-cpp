//
// Created by Cenxi Zhang on 2021/8/18.
//

#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGSERVICE_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGSERVICE_H

#include <domain/ElectricityReading.h>

#include <iostream>
#include <map>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>

class MeterReadingService {
 public:
  std::optional<std::vector<ElectricityReading>> getReadings(const std::string &smartMeterId) const {
    std::shared_lock<std::shared_mutex> lock(mtx_);
    if (meterAssociatedReadings_.find(smartMeterId) == meterAssociatedReadings_.end()) {
      return {};
    }
    return meterAssociatedReadings_[smartMeterId];
  }

  void storeReadings(const std::string &smartMeterId, std::vector<ElectricityReading> &electricityReadings) {
    std::unique_lock<std::shared_mutex> lock(mtx_);
    if (meterAssociatedReadings_.find(smartMeterId) == meterAssociatedReadings_.end()) {
      meterAssociatedReadings_[smartMeterId] = {};
    }
    meterAssociatedReadings_[smartMeterId].insert(meterAssociatedReadings_[smartMeterId].end(), electricityReadings.begin(),
                                                  electricityReadings.end());
  }

  explicit MeterReadingService(std::unordered_map<std::string, std::vector<ElectricityReading>> &meterAssociatedReadings)
      : meterAssociatedReadings_(meterAssociatedReadings) {}

 private:
  mutable std::shared_mutex mtx_;
  std::unordered_map<std::string, std::vector<ElectricityReading>> &meterAssociatedReadings_;
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_METERREADINGSERVICE_H
