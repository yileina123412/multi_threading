cmake_minimum_required(VERSION 3.16)#cmake版本  这里看自己的cmake版本
project(Mymutithread)  #工作空间名称
#设置C++标准为17
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug) #指定Release 构件时会优化

######外置的库  
## 
# find_package(Eigen3 REQUIRED)

# find_package(OpenMP REQUIRED)

find_package(Threads REQUIRED)

add_library(project_common INTERFACE)
target_include_directories(project_common INTERFACE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(project_common INTERFACE Threads::Threads)

##############################
##头文件搜索路径  可以从第三方库的CMake中找
include_directories(
#   ${EIGEN3_INCLUDE_DIR}
  # "${PROJECT_SOURCE_DIR}/include"   #源代码目录的头文件
  #系统的标准安装目录
  /usr/local/include # Location when using 'make system_install'
  /usr/include       # More usual location (e.g. when installing using a package)
)
#设置连接器查找库文件目录
link_directories(
  /usr/local/lib # Location when using 'make system_install'
  /usr/lib       # More usual location (e.g. when installing using a package)
)

add_subdirectory(lesson1)
add_subdirectory(lesson2)