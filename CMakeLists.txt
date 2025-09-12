cmake_minimum_required(VERSION 3.16)

# Set the project name
project(ShortRateModelCalibration)

# Set the C++ standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the include directory
include_directories(include)

# Add the executable
add_executable(
    Calibrator
    src/main.cpp
    src/HoLeeModel.cpp
    src/HullWhiteModel.cpp
    src/BDTModel.cpp
    src/BlackKarasinskiModel.cpp
)

# A message to show where the executable is
message(STATUS "Executable will be built in the build/ directory")
