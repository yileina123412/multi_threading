

add_executable(lesson1 main.cpp)

# 针对这个程序，单独指定头文件路径
target_include_directories(lesson1 PRIVATE "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(lesson1 PRIVATE project_common)
