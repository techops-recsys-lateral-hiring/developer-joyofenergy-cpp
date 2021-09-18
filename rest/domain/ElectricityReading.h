#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_ELECTRICITYREADING_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_ELECTRICITYREADING_H

#include <chrono>
#include <ctime>

class ElectricityReading {
 public:
  using time_point_type = std::chrono::time_point<std::chrono::system_clock>;

  ElectricityReading(time_point_type time, size_t reading) : time(time), reading(reading) {}

  time_point_type getTime() const { return time; }

  size_t getReading() const { return reading; }

 private:
  time_point_type time;
  size_t reading;
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_ELECTRICITYREADING_H
