#ifndef DEVELOPER_JOYOFENERGY_CPP_ELECTRICITYREADINGSERVICE_H
#define DEVELOPER_JOYOFENERGY_CPP_ELECTRICITYREADINGSERVICE_H

#include <domain/ElectricityReading.h>

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class ElectricityReadingService {
public:
    ElectricityReadingService(std::unordered_map<std::string, std::vector<ElectricityReading>> &meterAssociatedReadings)
            : meterAssociatedReadings(meterAssociatedReadings) {}

    std::optional<std::vector<ElectricityReading>> GetReading(const std::string &meterId) {
        auto found = meterAssociatedReadings.find(meterId);
        if (found != meterAssociatedReadings.end()) {
            return found->second;
        }
        return {};
    }

private:
    std::unordered_map<std::string, std::vector<ElectricityReading>> &meterAssociatedReadings;
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_ELECTRICITYREADINGSERVICE_H
