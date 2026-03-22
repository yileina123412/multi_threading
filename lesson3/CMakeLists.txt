

add_executable(lesson3_1 main3_1.cpp)
add_executable(lesson3_2 main3_2.cpp)

add_executable(main3_3 main3_3.cpp)

# 针对这个程序，单独指定头文件路径
target_include_directories(lesson3_1 PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(lesson3_1 PRIVATE project_common)

target_include_directories(lesson3_2 PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(lesson3_2 PRIVATE project_common)

target_include_directories(main3_3 PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(main3_3 PRIVATE project_common)

# target_include_directories(lesson2_3 PRIVATE "${PROJECT_SOURCE_DIR}/include")
# target_link_libraries(lesson2_3 PRIVATE project_common)

# target_include_directories(lesson2_4 PRIVATE "${PROJECT_SOURCE_DIR}/include")
# target_link_libraries(lesson2_4 PRIVATE project_common)
