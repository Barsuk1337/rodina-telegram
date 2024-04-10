# This file demonstrates how to compile the SAMPAC project on Linux.
#
# To compile SAMPAC do:
#
# make SAMPAC
# -DBOOST_ERROR_CODE_HEADER_ONLY

GPP = g++
GCC = gcc
OUTFILE_PATH = "bin/telegram_protect.so"

COMPILE_FLAGS = -m32 -fPIC -c -O2 -w -D LINUX -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -D PROJECT_NAME=\"telegram_protect\" -D SAMPGDK_AMALGAMATION -DSAMPGDK_CPP_WRAPPERS -D SUBHOOK_STATIC -DCURL_STATICLIB
LIBS = -L./Shared/libs -static -static-libgcc -lc -lgcc -ldl -lboost_system -lrt -lcurl -lssl -lcrypto

LDFLAGS = -shared -lpthread

TELEGRAM_PROTECT = -D TELEGRAM_PROTECT $(COMPILE_FLAGS)

all: TELEGRAM_PROTECT

clean:
	-rm -f *~ *.o *.so

quick:
	$(GPP) -m32 -O2 -fshort-wchar -static -o $(OUTFILE_PATH) *.o $(LDFLAGS) $(LIBS)

quickcomp:

	$(GPP) $(TELEGRAM_PROTECT) *.cpp
	quick

TELEGRAM_PROTECT: clean
	$(GPP) $(TELEGRAM_PROTECT) ./SDK/samp-sdk/*.cpp
	$(GCC) $(TELEGRAM_PROTECT) ./GDK/*.c
	$(GPP) $(TELEGRAM_PROTECT) *.cpp
	$(GPP) $(TELEGRAM_PROTECT) ./Shared/src/*.cpp
	$(GPP) $(TELEGRAM_PROTECT) ./Shared/src/net/*.cpp
	$(GPP) $(TELEGRAM_PROTECT) ./Shared/src/tools/*.cpp
	$(GPP) $(TELEGRAM_PROTECT) ./Shared/src/types/*.cpp
	mkdir -p "bin"
	$(GPP) -m32 -O2 -fshort-wchar -static -I ./Shared/ -o $(OUTFILE_PATH) *.o $(LDFLAGS) $(LIBS)
