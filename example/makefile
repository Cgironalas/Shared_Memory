CC=gcc

LDFLAGS= -lpthread -lm

time: time.o
	$(CC) time.o -o time $(LDFLAGS)

time.o: time.c
	$(CC) -c time.c -o time.o $(LDFLAGS)

clean:
	rm -f *.o
	rm -f time
	