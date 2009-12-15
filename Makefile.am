bin_PROGRAMS = cy3240_i2c runTests

cy3240_i2c_SOURCES = \
	src/main.c \
	src/cy3240.c \
	src/cy3240.h \
	src/cy3240_util.c \
	src/cy3240_util.h \
	src/cy3240_debug.c \
	src/cy3240_debug.h \
	src/cy3240_packet.h \
	src/cy3240_types.h \
	src/i2c_demo.h

cy3240_i2c_LDADD= -lusb -lhid

AM_CPPFLAGS=-I aceunit/src/native -I src -DACEUNIT_SUITES

runTests_SOURCES = \
	src/tests/Suite1.c \
	src/tests/writeTest.c \
	src/tests/writeTest.h \
	src/cy3240.c \
	src/cy3240.h \
	src/cy3240_util.c \
	src/cy3240_util.h \
	src/cy3240_debug.c \
	src/cy3240_debug.h \
	src/cy3240_packet.h \
	src/cy3240_types.h \
	src/i2c_demo.h \
	src/i2c_demo.h \
	aceunit/src/native/AceUnit.c \
	aceunit/src/native/AceUnit.h \
	aceunit/src/native/AceUnitAssert.h \
	aceunit/src/native/AceUnitData.c \
	aceunit/src/native/AceUnitData.h \
	aceunit/src/native/FullPlainLogger.c \
	aceunit/src/native/AceUnitAnnotations.h \
	aceunit/src/native/AceUnitMain.c \
	aceunit/src/native/AceUnitLogging.h \
	aceunit/src/native/ExceptionHandling.c \
	aceunit/src/native/ExceptionHandling.h

# These are only used for reference
runTests_LDADD= -lusb -lhid
