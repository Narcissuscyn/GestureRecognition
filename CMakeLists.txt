cmake_minimum_required(VERSION 3.10)
project(GESTURE_RECOGNITION)
add_executable(gesture_recognition  diff_clic.cpp utils.h gesture.h my_code.cpp Point.h latest.cpp)


message("----------- trying to find Jsoncpp-------------")
#include_directories( /usr/local/include/jsoncpp )

TARGET_LINK_LIBRARIES(gesture_recognition jsoncpp)

find_package(OpenCV)
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries(gesture_recognition ${OpenCV_LIBS})
