# This file demonstrates how to compile the AUXAC project on Linux.
#
# To compile AUXAC do:
#
# make AUXAC
#

GPP = g++-4.8 -m32 -c -O2 -w -std=c++11 -Ilib -fno-stack-protector -fpermissive
GCC = gcc-4.8 -m32 -Ilib -fno-stack-protector -fpermissive
LINK = $(GCC)
AUXAC_OUTFILE = "../../bin/server/server.so"

COMPILE_FLAGS = -c -O3 -w -LINUX -D SUBHOOK_STATIC -D SAMPGDK_AMALGAMATION -D LINUX

AUXAC = -D AUXAC $(COMPILE_FLAGS)

all: AUXAC

clean:
	-rm -rf *~ *.o *.so

AUXAC: clean
	$(GPP) $(AUXAC) ./lib/samp-plugin-sdk/*.cpp
	$(GPP) $(AUXAC) ./lib/RakNet/*.cpp
	$(GCC) $(AUXAC) ./lib/sampgdk/sampgdk.c
	$(GCC) $(AUXAC) ./lib/subhook/subhook.c
	$(GPP) $(AUXAC) ./*.cpp
	$(LINK) -lpthread -shared -o $(AUXAC_OUTFILE) *.o
	-rm -rf *~ *.o