# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\PIC-Simulator_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\PIC-Simulator_autogen.dir\\ParseCache.txt"
  "PIC-Simulator_autogen"
  )
endif()
