cmake_minimum_required(VERSION 3.15)
project(testOpenCV)
set(TEST_OPENCV ${PROJECT_NAME})

#set(OpenCV_DIR ${CMAKE_CURRENT_LIST_DIR}/opencv/x64)

#set(OpenCV_DIR D:/projects/ultras/libs/opencv/x64)
#set(OpenCV_DIR E:/code/UltraSound/Registration/libs/opencv/x64)

set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/opencv/x64)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_PREFIX_PATH /home/zj/opencv/opencv-4.2.0/install)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})


MESSAGE("OpenCV version: ${OpenCV_VERSION}")
MESSAGE("OpenCV OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
MESSAGE("OpenCV OpenCV_LIBS: ${OpenCV_LIBS}")

add_executable(${TEST_OPENCV} testOpenCV.cpp)
target_link_libraries(${TEST_OPENCV} ${OpenCV_LIBS})