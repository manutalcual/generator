#
#
#
AM_CPPFLAGS =  -Wall -W -DG_LOG_DOMAIN=\"generator\"

# -D_REENTRANT -fPIE
# -D_QX_ENABLE_BOOST_SERIALIZATION \
# -D_QX_ENABLE_BOOST_SERIALIZATION_BINARY \
# -D_QX_ENABLE_BOOST_SERIALIZATION_XML \
# -D_QX_MODE_DEBUG -DQT_SQL_LIB -DQT_CORE_LIB \
# -I$(top_srcdir) \
# -I$(top_srcdir)/include \
# -I/home/manuel/by/hulk/libs/boost/include \
# -I/home/manuel/by/hulk/bytech-hulk/bytech-hulk-entities/include \
# -I/home/manuel/by/hulk/exteral/QxOrm/include \
# -I/home/manuel/Qt/5.3/gcc_64/include

#AM_LDFLAGS = 
# -Wl,-rpath,/home/manuel/Qt/5.3/gcc_64 -Wl,-rpath,/home/manuel/Qt/5.3/gcc_64/lib \
# -Wl,-rpath,/home/manuel/by/hulk/libs/boost/lib \
# -Wl,-rpath,/home/manuel/by/hulk/libs/qxorm/lib \
# -Wl,-rpath,/home/manuel/by/hulk/bytech-hulk/bytech-hulk-entities/bin \
# -L/home/manuel/by/hulk/libs/boost/lib \
# -L/home/manuel/by/hulk/libs/qxorm/lib \
# -L/home/manuel/by/hulk/bytech-hulk/bytech-hulk-entities/bin \
# -L/home/manuel/Qt/5.3/gcc_64/lib

#LIBS = -lboost_serialization  -lQxOrmd -lHulkd  -lQt5Sql -lQt5Core -lpthread
#-licui18n -licuuc -licudata -lQt5Gui

bin_PROGRAMS = generator lamb
generator_SOURCES = common.hh common.cc \
	opt.hh opt.cc \
	base_parser.hh base_parser.cc \
	conf.hh conf.cc \
	stat.hh stat.cc \
	file.hh file.cc \
	map_file.hh map_file.cc \
	comandos.hh comandos.cc \
	parser.hh parser.cc \
	main.hh main.cc

lamb_SOURCES = lamb.hh lamb.cc

#	generator.hh generator.cc \
#
# db_SOURCES = common.hh common.cc \
# 	opt.hh opt.cc \
# 	db.hh db.cc
