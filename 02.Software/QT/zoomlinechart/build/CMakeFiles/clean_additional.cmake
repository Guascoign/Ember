# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\zoomlinechart_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\zoomlinechart_autogen.dir\\ParseCache.txt"
  "zoomlinechart_autogen"
  )
endif()
