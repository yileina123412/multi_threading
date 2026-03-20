

add_executable(lesson2 main.cpp)
add_executable(lesson2_3 main2_3.cpp)
add_executable(lesson2_4 main2_4.cpp)


# 针对这个程序，单独指定头文件路径
target_include_directories(lesson2 PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(lesson2 PRIVATE project_common)

target_include_directories(lesson2_3 PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(lesson2_3 PRIVATE project_common)

target_include_directories(lesson2_4 PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(lesson2_4 PRIVATE project_common)
