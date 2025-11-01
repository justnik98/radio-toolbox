cmake_minimum_required(VERSION 4.0)
project(radio_toolbox)

set(CMAKE_CXX_STANDARD 20)

add_executable(radio_toolbox main.cpp
        modulators/IModulator.h
        modulators/QamMod.cpp
        modulators/QamMod.h
        modulators/PmMod.cpp
        modulators/PmMod.h
        modulators/IModulator.cpp
)
