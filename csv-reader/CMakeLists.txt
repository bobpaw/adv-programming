add_library(csv OBJECT csv.cc csv.h)
target_compile_features(csv PUBLIC cxx_decltype)

add_executable(reader main.cc csv)

add_custom_command(TARGET reader POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/life_expectancy_asc.csv" "$<TARGET_FILE_DIR:reader>")