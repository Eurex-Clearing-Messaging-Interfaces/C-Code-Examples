add_definitions (-std=c++11)
set(CMAKE_CXX_FLAGS "-std=c++11")
find_package (ProtonCpp REQUIRED)
include_directories (${ProtonCpp_INCLUDE_DIRS})
add_library (${PROJECT_NAME} BroadcastReceiver.cpp RequestResponse.cpp)
add_executable (demo_qpid-proton main.cpp)
target_link_libraries (demo_qpid-proton
                       ${PROJECT_NAME}
                       ${ProtonCpp_LIBRARIES})
