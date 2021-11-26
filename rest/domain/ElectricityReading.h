#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_ELECTRICITYREADING_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_ELECTRICITYREADING_H

#include <chrono>
#include <ctime>

class ElectricityReading {
 public:
  using time_point_type = std::chrono::time_point<std::chrono::system_clock>;

  ElectricityReading(time_point_type time, size_t reading) : time_(time), reading_(reading) {}

  time_point_type getTime() const { return time_; }

  size_t getReading() const { return reading_; }

  bool operator==(const ElectricityReading& rhs) const { return time_ == rhs.time_ && reading_ == rhs.reading_; }
  bool operator!=(const ElectricityReading& rhs) const { return !(rhs == *this); }

 private:
  time_point_type time_;
  size_t reading_;  // scale out in 0.1w for precision
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_ELECTRICITYREADING_H
