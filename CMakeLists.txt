cmake_minimum_required(VERSION 2.8.12)

project(HttpServer)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# TODO make theese required?
find_package(Qt5Core)
find_package(Qt5Network)
find_package(Config)
if(NOT CONFIG++_FOUND)
    message( FATAL_ERROR "Install libconfig with \"apt-get install libconfig++-dev\"." )
endif()


add_executable(${PROJECT_NAME}
    "main.cpp"
    "Server.cpp"
    "Server.h"
    "ResponseWorker.cpp"
    "ResponseWorker.h"
    "HttpResponse.cpp"
    "HttpResponse.h"
    "Logger.cpp"
    "Logger.h"
    )

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network config++)

install (TARGETS ${PROJECT_NAME} DESTINATION bin)
set(INSTALLED_EXECUTABLE ${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME})
install(CODE "execute_process(COMMAND chown root ${INSTALLED_EXECUTABLE})")
install(CODE "execute_process(COMMAND chmod 6555 ${INSTALLED_EXECUTABLE})")
set(JAIL_SCRIPT "create_php_jail.sh")
set(INSTALLED_SCRIPT ${CMAKE_INSTALL_PREFIX}/bin/${JAIL_SCRIPT})
install(FILES ${PROJECT_SOURCE_DIR}/${JAIL_SCRIPT} DESTINATION bin)
install(CODE "execute_process(COMMAND chmod 555 ${INSTALLED_SCRIPT})")
