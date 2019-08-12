cmake_minimum_required (VERSION 3.0)

enable_testing()

include_directories (${PROJECT_SOURCE_DIR}/src)
add_subdirectory (src)

include_directories (${PROJECT_SOURCE_DIR}/test)
add_subdirectory (test)
