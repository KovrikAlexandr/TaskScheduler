add_library(${PROJECT_NAME} TaskScheduler.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++20)