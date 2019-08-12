cmake_minimum_required (VERSION 3.0)

enable_testing()

include_directories (${PROJECT_SOURCE_DIR}/source)
add_subdirectory (source)

include_directories (${PROJECT_SOURCE_DIR}/test)
add_subdirectory (test)
