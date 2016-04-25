add_definitions(-std=c++11)
find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
find_package (Threads REQUIRED)
include_directories (${PROJECT_SOURCE_DIR}/src
                     ${Boost_INCLUDE_DIRS}
                     )
add_definitions (-DBOOST_TEST_DYN_LINK)
add_executable (test_Broadcaster test_Broadcaster.cpp Broadcaster.cpp)
add_executable (test_Responder test_Responder.cpp Responder.cpp)
target_link_libraries (test_Broadcaster
                       ${PROJECT_NAME}
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       qpidclient
                       qpidtypes
                       qpidmessaging
                       Threads::Threads
                       )
target_link_libraries (test_Responder
                       ${PROJECT_NAME}
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       qpidclient
                       qpidtypes
                       qpidmessaging
                       Threads::Threads
                       )
