cmake_minimum_required(VERSION 3.15)
project(testOpenCV)

set(OpenCV_DIR E:/code/UltraSound/Registration/libs/opencv/x64)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_PREFIX_PATH /home/zj/opencv/opencv-4.2.0/install)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})


MESSAGE("OpenCV version: ${OpenCV_VERSION}")
MESSAGE("OpenCV OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
MESSAGE("OpenCV OpenCV_LIBS: ${OpenCV_LIBS}")

add_executable(testOpenCV testOpenCV.cpp)
target_link_libraries(testOpenCV ${OpenCV_LIBS})