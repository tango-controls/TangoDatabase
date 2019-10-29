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

if(NOT TANGO_DB_NAME)
    message("No Tango database name specified using -DTANGO_DB_NAME - default is tango")
    set(TANGO_DB_NAME tango)
endif()
if(NOT MYSQL)
    message("No MySQL client program specified using -DMYSQL - default is mysql")
    set(MYSQL mysql)
endif()
if(NOT MYSQL_ADMIN)
    message("No MySQL admin user name specified using -DMYSQL_ADMIN")
endif()
if(NOT MYSQL_ADMIN_PASSWD)
    message("No MySQL admin password specified using -DMYSQL_ADMIN_PASSWD")
endif()
if(NOT MYSQL_HOST)
    message("No MySQL hostname specified using -DMYSQL_HOST")
endif()

set(DB_SCRIPTS create_db.sh
               create_db.sql
               create_db_tables.sql
               rem_history.sql
               stored_proc.sql
               update_db_from_5_to_9.3.4.sql
               update_db_from_6_to_9.3.4.sql
               update_db_from_7_to_9.3.4.sql
               update_db_from_8_to_9.3.4.sql
               update_db_from_9.2.5_to_9.3.4.sql
               update_db.sh
               update_db.sql)

foreach(DB_SCRIPT_FILE ${DB_SCRIPTS})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${DB_SCRIPT_FILE}.in ${CMAKE_CURRENT_BINARY_DIR}/${DB_SCRIPT_FILE})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DB_SCRIPT_FILE} DESTINATION "${CMAKE_INSTALL_PREFIX}/share/tango/db")
endforeach(DB_SCRIPT_FILE)
