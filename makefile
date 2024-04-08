# This file demonstrates how to compile the TELEGRAM_PROTECT project on Linux.
#
# To compile TELEGRAM_PROTECT do:
#
# make TELEGRAM_PROTECT
# -DBOOST_ERROR_CODE_HEADER_ONLY

GPP = g++
GCC = gcc
OUTFILE_PATH = "bin/telegram_protect.so"

COMPILE_FLAGS = -m32 -fPIC -c -O2 -w -D LINUX -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -D PROJECT_NAME=\"telegram_protect\" -D SAMPGDK_AMALGAMATION -DSAMPGDK_CPP_WRAPPERS -D SUBHOOK_STATIC -DCURL_STATICLIB -DTGBOT_DISABLE_NAGLES_ALGORITHM -DTGBOT_CHANGE_SOCKET_BUFFER_SIZE -DTGBOT_CHANGE_READ_BUFFER_SIZE -DBOOST_BIND_GLOBAL_PLACEHOLDERS
LIBS = -L./Shared/libs -static -static-libgcc -lc -lgcc -ldl -lboost_system -lboost_thread -lboost_filesystem -lboost_chrono -lboost_regex -lboost_date_time -lrt -lsubhook -lcurl -lssl -lcrypto

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
	$(GPP) $(TELEGRAM_PROTECT) ./Shared/MD5_Info/*.cpp
	mkdir -p "bin"
	$(GPP) -m32 -O2 -fshort-wchar -static -I ./Shared/ -static-libstdc++ -o $(OUTFILE_PATH) *.o $(LDFLAGS) $(LIBS)
