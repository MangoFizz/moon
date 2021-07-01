# SPDX-License-Identifier: GPL-3.0-only

add_library(test MODULE
    src/test/test.c
)

# Set output name
set_target_properties(test PROPERTIES PREFIX "")
set_target_properties(test PROPERTIES OUTPUT_NAME "test")

# Set linker flags
set_target_properties(test PROPERTIES LINK_FLAGS "-static -static-libgcc -static-libstdc++")
