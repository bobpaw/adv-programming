add_library(csv OBJECT csv.cc csv.h)
target_compile_features(csv PUBLIC cxx_decltype)

add_executable(reader main.cc csv)
