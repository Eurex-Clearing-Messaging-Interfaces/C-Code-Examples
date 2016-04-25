add_definitions(-std=c++11)
add_library (${PROJECT_NAME} BroadcastReceiver.cpp RequestResponse.cpp)
add_executable (demo main.cpp)
target_link_libraries (demo qpidclient qpidtypes qpidmessaging ${PROJECT_NAME})
