CC = $(CROSS_COMPILE)gcc

config ?= debug

ifeq (,$(filter release,$(config)))
  CFLAG += -g
endif
CFLAG += -W

RMI_DIR := ../../librmi_src
BIN_DIR := ../../bin
RMI_LIB_DIR := ../../lib

LIB_DIR := -L$(RMI_LIB_DIR)
INC_DIRS := -I$(RMI_DIR)

LIBS := $(RMI_LIB_DIR)/librmi.a -lpthread -lrt

.PHONY: all clean 

all: main
	
rmiparse_middleman : test.h
	make -C $(RMI_DIR)
	$(BIN_DIR)/rmiparse test.h test
	#cp ../lib/librmi.so .
	@touch rmiparse_middleman

main: rmiparse_middleman main.c
	$(CC) -o $@ main.c test_stub.c $(LIBS) $(LIB_DIR) $(INC_DIRS) $(CFLAG)
		
clean:
	-rm -f main *.so *.a
	-rm rmiparse_middleman
	-rm test_stub.c test_proxier.c test_rmi.h
	make -C $(RMI_DIR) clean
