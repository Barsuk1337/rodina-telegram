GPP = g++
GCC = gcc
OUTFILE_PATH = "bin/telegram_protect.so"

COMPILE_FLAGS = -m32 -fPIC -c -O2 -w -D LINUX -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -D PROJECT_NAME=\"telegram_protect\" -D SAMPGDK_AMALGAMATION -DSAMPGDK_CPP_WRAPPERS
LIBS = -L./include/libs -static -static-libgcc -lgcc -lssl -lcrypto

LDFLAGS = -shared -lpthread

TELEGRAM_PROTECT = -D TELEGRAM_PROTECT $(COMPILE_FLAGS)

all: TELEGRAM_PROTECT

clean:
	-rm -f *~ *.o *.so

TELEGRAM_PROTECT: clean
	$(GPP) $(TELEGRAM_PROTECT) ./include/SDK/samp-sdk/*.cpp
	$(GCC) $(TELEGRAM_PROTECT) ./include/GDK/*.c
	$(GPP) $(TELEGRAM_PROTECT) -std=c++17 ./include/tgbot/*.cpp
	$(GPP) $(TELEGRAM_PROTECT) -std=c++17 ./include/tgbot/net/*.cpp
	$(GPP) $(TELEGRAM_PROTECT) -std=c++17 ./include/tgbot/tools/*.cpp
	$(GPP) $(TELEGRAM_PROTECT) -std=c++17 ./include/tgbot/types/*.cpp
	$(GPP) $(TELEGRAM_PROTECT) -I ./include/ ./include/*.cpp
	$(GPP) $(TELEGRAM_PROTECT) -I ./include/ ./src/*.cpp
	mkdir -p "bin"
	$(GPP) -m32 -O2 -fshort-wchar -static -o $(OUTFILE_PATH) *.o $(LDFLAGS) $(LIBS)
	rm *.o
