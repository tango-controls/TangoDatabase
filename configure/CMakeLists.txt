include(FindPkgConfig)
include(GNUInstallDirs)
include(configure/FindMySQL.cmake)

if(NOT CMAKE_BUILD_TYPE)
    message("No build type specified - default is DEBUG")
    set(CMAKE_BUILD_TYPE DEBUG)
endif()


if(NOT MYSQL_FOUND)
    message(SEND_ERROR " Can not find MySQL client library.")
endif()

pkg_search_module(TANGO_PKG REQUIRED tango)