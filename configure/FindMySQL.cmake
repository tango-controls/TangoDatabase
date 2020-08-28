# - Try to find MySQL.
# Once done this will define:
# MYSQL_FOUND			- If false, do not try to use MySQL.
# MYSQL_INCLUDE_DIRS	- Where to find mysql.h, etc.
# MYSQL_LIBRARIES		- The libraries to link against.
#

if( WIN32 )
    find_path( MYSQL_INCLUDE_DIR
            NAMES "mysql.h"
            PATHS "$ENV{PROGRAMFILES}/MySQL/*/include"
            "$ENV{PROGRAMFILES(x86)}/MySQL/*/include"
            "$ENV{SYSTEMDRIVE}/MySQL/*/include" )

    find_library( MYSQL_LIBRARY
            NAMES "mysqlclient" "mysqlclient_r"
            PATHS "$ENV{PROGRAMFILES}/MySQL/*/lib"
            "$ENV{PROGRAMFILES(x86)}/MySQL/*/lib"
            "$ENV{SYSTEMDRIVE}/MySQL/*/lib" )
else()
    find_path( MYSQL_INCLUDE_DIR
            NAMES "mysql.h"
            PATHS "/usr/include/mysql"
            "/usr/include/mariadb"
            "/usr/local/include/mysql"
            "/usr/mysql/include/mysql" )

    find_library( MYSQL_LIBRARY
            NAMES "mysqlclient" "mysqlclient_r"
            PATHS "/lib/mysql"
            "/lib64/mysql"
            "/usr/lib/mysql"
            "/usr/lib/mariadb"
            "/usr/lib64/mysql"
            "/usr/local/lib/mysql"
            "/usr/local/lib64/mysql"
            "/usr/mysql/lib/mysql"
            "/usr/mysql/lib64/mysql" )
endif()

# handle the QUIETLY and REQUIRED arguments and set MYSQL_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )

find_package_handle_standard_args( MYSQL DEFAULT_MSG MYSQL_LIBRARY MYSQL_INCLUDE_DIR)

set( MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR} )
set( MYSQL_LIBRARIES ${MYSQL_LIBRARY} )

mark_as_advanced( MYSQL_INCLUDE_DIR MYSQL_LIBRARY )
