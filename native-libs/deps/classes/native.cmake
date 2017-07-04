# Where is the target environment:
set(CMAKE_FIND_ROOT_PATH  "${CMAKE_INSTALL_PREFIX}")

# Search for programs in the build host directories:
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search for libraries and headers in the target directories:
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
