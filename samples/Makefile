CC = $(CROSS_COMPILE)gcc

config ?= debug

ifeq (,$(filter release,$(config)))
  CFLAG += -g
endif
CFLAG += -W

RMI_DIR := ../librmi_src

LIB_DIR := -L../lib
BIN_DIR := -L../bin
INC_DIRS := -I../librmi_src

LIBS := ../lib/librmi.a -lpthread -lrt

.PHONY: all clean 

all: server client
	
rmiparse_middleman : test.h
	make -C $(RMI_DIR)
	../bin/rmiparse test.h test
	../bin/rmiparse find_device.h find
	#cp ../lib/librmi.so .
	@touch rmiparse_middleman

server: rmiparse_middleman server.c
	$(CC) -o $@ server.c test_stub.c find_proxier.c $(LIBS) $(LIB_DIR) $(INC_DIRS) $(CFLAG)
	
client: rmiparse_middleman client.c
	$(CC) -o $@ client.c test_proxier.c find_stub.c $(LIBS) $(LIB_DIR) $(INC_DIRS) $(CFLAG)
	
clean:
	-rm -f server client *.so *.a
	-rm test_stub.c test_proxier.c rmiparse_middleman
	make -C $(RMI_DIR) clean
