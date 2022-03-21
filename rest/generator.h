#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_GENERATOR_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_GENERATOR_H

#include <domain/ElectricityReading.h>

#include <vector>

class Generator {
 public:
  std::vector<ElectricityReading> generate(int number) {
    std::vector<ElectricityReading> readings;
    readings.reserve(number);
    auto now = std::chrono::system_clock::now();
    for (int i = number; i > 0; i--) {
      auto r = std::abs(std::rand()) % 4000;
      readings.emplace_back(now - std::chrono::minutes(i * 3), 4000);
    }
    return readings;
  }
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_GENERATOR_H
