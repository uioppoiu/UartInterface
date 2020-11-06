cmake_minimum_required(VERSION 3.0.0)
project(UART VERSION 0.1.0)

add_executable(UART
    main.cpp
    tinyxml2/tinyxml2.h tinyxml2/tinyxml2.cpp
    MessageInterface.h MessageInterface.cpp)

include_directories(tinyxml2)

# add_subdirectory(test)