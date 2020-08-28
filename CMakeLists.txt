cmake_minimum_required(VERSION 2.8.9)
project(DataBase)

include(configure/CMakeLists.txt)

# on change also adapt the versions in Makefile line 30ff
set(MAJOR_VERSION "5")
set(MINOR_VERSION "16")
set(PATCH_VERSION "0")
set(PROJECT_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")

set(SOURCES DataBase.cpp
            DataBaseClass.cpp
            DataBaseStateMachine.cpp
            ClassFactory.cpp
            main.cpp)

set(ADDITIONAL_SOURCES  DataBaseUtils.cpp
                        update_starter.cpp)

include_directories("." ${TANGO_PKG_INCLUDE_DIRS} ${MYSQL_INCLUDE_DIRS})
link_directories(${TANGO_PKG_LIBRARY_DIRS})

add_executable(Databaseds ${SOURCES} ${ADDITIONAL_SOURCES})
target_link_libraries(Databaseds ${TANGO_PKG_LIBRARIES} ${MYSQL_LIBRARIES} -Wl,-z,now -pie)
target_compile_options(Databaseds PUBLIC ${TANGO_PKG_CFLAGS_OTHER} -Wall -Wextra -D_FORTIFY_SOURCE=2 -O1 -fpie)

install(TARGETS Databaseds
        RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}"
        CONFIGURATIONS ${CMAKE_BUILD_TYPE})
