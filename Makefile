CFLAGS  = -Wall -std=c11 -g
LIBS = -lm

.PHONY: clean 

OBJS =  db.o main.o

all : db

REBUILDABLES = $(OBJS) $(APP)

clean :
	rm -f *.o  *~ db tdb test.bin

test : db.o test.o
	gcc $(CFLAGS) -o tdb db.o test.o $(LIBS)

db : db.o test.o
	gcc $(CFLAGS) -o db db.o main.c $(LIBS)
