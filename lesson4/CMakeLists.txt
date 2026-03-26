
add_executable(lesson4_1 main4_1.cpp)
add_executable(lesson4_2 main4_2.cpp)
add_executable(lesson4_3 main4_3.cpp)
add_executable(lesson4_4 main4_4.cpp)



# 针对这个程序，单独指定头文件路径
target_include_directories(lesson4_1 PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(lesson4_1 PRIVATE project_common)

target_include_directories(lesson4_2 PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(lesson4_2 PRIVATE project_common)

target_include_directories(lesson4_3 PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(lesson4_3 PRIVATE project_common)

target_include_directories(lesson4_4 PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(lesson4_4 PRIVATE project_common)



