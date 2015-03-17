CC = $(CROSS_COMPILE)gcc

config ?= debug

ifeq (,$(filter release,$(config)))
  CFLAG += -g
endif
CFLAG += -W

OUT_DIR := ../lib

.PHONY: all clean 

all: librmi.so librmi.a

%.o : %.c
	$(CC) $(CFLAG) -c -o $@ $<

librmi.a : debug.o socket.o rmi.o list.o mem_pool.o
	ar rcs $(OUT_DIR)/$@ $^
	
librmi.so : debug.o socket.o rmi.o list.o mem_pool.o
	$(CC) -shared -fPIC -o $(OUT_DIR)/$@ $^
	
clean:
	-rm -f *.o
	-rm -f $(OUT_DIR)/*.a $(OUT_DIR)/*.so
