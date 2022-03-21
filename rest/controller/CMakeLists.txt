add_library(controller INTERFACE)
find_package(Boost REQUIRED)
# Turn off magic autolink esp for Windows
target_compile_definitions(Boost::headers INTERFACE BOOST_ALL_NO_LIB)
find_package(nlohmann_json REQUIRED)
target_link_libraries(controller INTERFACE Boost::headers nlohmann_json::nlohmann_json service)
