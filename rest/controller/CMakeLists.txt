add_library(controller INTERFACE)
find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
target_link_libraries(controller INTERFACE Boost::headers nlohmann_json::nlohmann_json service)
