# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/SimpleLighting_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/SimpleLighting_autogen.dir/ParseCache.txt"
  "SimpleLighting_autogen"
  )
endif()
