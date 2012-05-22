INCLUDES = $(ORB_INCLUDE_PREFIX)  $(LIBZMQ_CFLAGS) \
	   -I$(top_srcdir)/lib/cpp/log4tango/include \
	   -I$(top_builddir)/lib/cpp/log4tango/include \
	   -I$(top_srcdir)/lib/cpp/client \
	   -I$(top_srcdir)/lib/cpp/server \
      	$(MYSQLCLIENT_CFLAGS) \
      	$(ZLIB_CPPFLAGS)

AM_CXXFLAGS= @CPP_ELEVEN@ -Wall -D_FORTIFY_SOURCE=2 -O1 -fpie

LDADD = -L$(top_builddir)/lib/cpp/server -ltango -L$(top_builddir)/lib/cpp/log4tango/src \
		-llog4tango $(MYSQLCLIENT_LDFLAGS) \
         $(MYSQLCLIENT_LIBS) $(ZLIB_LDFLAGS) $(ZLIB_LIBS)  $(LIBZMQ_LIBS)

AM_LDFLAGS = -Wl,-z,now -pie 

bin_PROGRAMS=DataBaseds

DataBaseds_SOURCES=ClassFactory.cpp  		\
                   DataBaseClass.cpp 		\
                   DataBase.cpp      		\
				   DataBaseStateMachine.cpp \
                   main.cpp          		\
                   update_starter.cpp		\
				   DataBaseUtils.cpp		\
				   DataBase.h				\
				   DataBaseClass.h 			\
		  		   update_starter.h

if TANGO_DB_CREATE_ENABLED

dbdir=${pkgdatadir}/db
db_DATA=create_db.sh create_db.sql my.cnf create_db_tables.sql stored_proc.sql update_db.sh update_db6.sql
	
## This is to make sure that the create-db script is run on each make all.
## See create_db.sh for more information.
all-local: .force
	$(SHELL) ${top_builddir}/cppserver/database/create_db.sh < ${top_builddir}/cppserver/database/create_db.sql

endif

EXTRA_DIST	= create_db.sh.in create_db.sql.in my.cnf.in create_db_tables.sql.in stored_proc.sql.in \
				update_db.sh.in update_db6.sql.in	

.force:
	
