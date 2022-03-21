add_library(service INTERFACE)
target_include_directories(service INTERFACE ..)
target_link_libraries(service INTERFACE domain)
