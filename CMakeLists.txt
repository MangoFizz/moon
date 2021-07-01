# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.16)

# Set project info
project(Moon
    DESCRIPTION "DInput8 mod loader"
    VERSION 1.0.0
    LANGUAGES C CXX
)

add_library(dinput8 MODULE
    src/dinput8.cpp
    src/dinput8.def
)

# Set output name
set_target_properties(dinput8 PROPERTIES PREFIX "")
set_target_properties(dinput8 PROPERTIES OUTPUT_NAME "DInput8")

# Set linker flags
set_target_properties(dinput8 PROPERTIES LINK_FLAGS "-static -static-libgcc -static-libstdc++")

# Build test
include(src/test/test.cmake)
