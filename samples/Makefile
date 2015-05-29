CC = $(CROSS_COMPILE)gcc

config ?= debug

ifeq (,$(filter release,$(config)))
  CFLAG += -g
endif
CFLAG += -W

LIB_DIR := -L../lib
BIN_DIR := -L../bin
INC_DIRS := -I../librmi_src

LIBS := ../lib/librmi.a -lpthread -lrt

.PHONY: all clean 

all: server client
	
rmiparse_middleman : test.h
	../bin/rmiparse test.h
	#cp ../lib/librmi.so .
	@touch rmiparse_middleman

server: rmiparse_middleman server.c
	$(CC) -o $@ server.c stub.c $(LIBS) $(LIB_DIR) $(INC_DIRS) $(CFLAG)
	
client: rmiparse_middleman client.c
	$(CC) -o $@ client.c proxier.c $(LIBS) $(LIB_DIR) $(INC_DIRS) $(CFLAG)
	
clean:
	-rm -f server client *.so *.a
	-rm stub.c proxier.c rmiparse_middleman
