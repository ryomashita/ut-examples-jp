add_executable(ex1 ex1.cpp)
target_include_directories(ex1 PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(readme readme_sample.cpp)
target_include_directories(readme PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(cfg_reporter cfg/reporter.cpp)
target_include_directories(cfg_reporter PUBLIC ${PROJECT_SOURCE_DIR}/include)