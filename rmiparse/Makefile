config ?= debug

ifeq (,$(filter release,$(config)))
  CFLAG += -g
endif
CFLAG += -W

OBJS := list.o rmiparse.o debug.o crc32.o

LIBS := -lrt
OUT_DIR := ../bin

CC = $(CROSS_COMPILE)gcc

.PHONY: all clean 

all: rmiparse

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAG)

rmiparse: lex_middleman bison_middleman $(OBJS)
	$(CC) -o $(OUT_DIR)/$@ c_grammer.yy.c c_grammer.tab.c $(OBJS) $(LIBS)
	
lex_middleman: c_grammer.l
	flex -o c_grammer.yy.c c_grammer.l
	@touch lex_middleman
	
bison_middleman: c_grammer.y
	bison -d -o c_grammer.tab.c c_grammer.y
	@touch bison_middleman
	
clean:
	-rm $(OBJS)
	-rm -f $(OUT_DIR)/rmiparse 
	-rm -f lex_middleman bison_middleman c_grammer.yy.c c_grammer.tab.c c_grammer.tab.h 
