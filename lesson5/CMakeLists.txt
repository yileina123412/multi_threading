
add_executable(lesson5_1 main5_1.cpp)
# 针对这个程序，单独指定头文件路径
target_include_directories(lesson5_1 PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(lesson5_1 PRIVATE project_common)

add_executable(main5_2 main5_2.cpp)
# 针对这个程序，单独指定头文件路径
target_include_directories(main5_2 PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(main5_2 PRIVATE project_common)

add_executable(main5_3 main5_3.cpp)
# 针对这个程序，单独指定头文件路径
target_include_directories(main5_3 PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(main5_3 PRIVATE project_common)




