project(aMAZEd)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libgcc")
cmake_minimum_required(VERSION 3.8)

# Where to find CMake modules and OpenCV
set(OpenCV_DIR "C:\\opencv\\opencv-3.2.0\\build")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(aMAZEd main.cpp)

# add OpenCV libs
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs opencv_videoio)

# linking
target_link_libraries(aMAZEd ${OpenCV_LIBS})