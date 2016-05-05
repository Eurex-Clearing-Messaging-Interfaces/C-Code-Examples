add_definitions(-std=c++11)
add_library (${PROJECT_NAME} BroadcastReceiver.cpp RequestResponse.cpp)
add_executable (demo_qpid-messaging main.cpp)
target_link_libraries (demo_qpid-messaging ${PROJECT_NAME} qpidmessaging qpidclient qpidtypes)
