CC=gcc

LDFLAGS= -lpthread -lm

starter: starter.o
	$(CC) starter.o -o starter $(LDFLAGS)
reader: reader.o
	$(CC) reader.o -o reader $(LDFLAGS)
greedy_reader: greedy_reader.o
	$(CC) greedy_reader.o -o greedy_reader $(LDFLAGS)
writer: writer.o
	$(CC) writer.o -o writer $(LDFLAGS)
spy: spy.o
	$(CC) spy.o -o spy $(LDFLAGS)
finisher: finisher.o
	$(CC) finisher.o -o finisher $(LDFLAGS)

starter.o: starter.c
	$(CC) -c starter.c -o starter.o $(LDFLAGS)
reader.o: reader.c
	$(CC) -c reader.c -o reader.o $(LDFLAGS)
greedy_reader.o: greedy_reader.c
	$(CC) -c greedy_reader.c -o greedy_reader.o $(LDFLAGS)
writer.o: writer.c
	$(CC) -c writer.c -o writer.o $(LDFLAGS)
spy.o: spy.c
	$(CC) -c spy.c -o spy.o $(LDFLAGS)
finisher.o: finisher.c
	$(CC) -c finisher.c -o finisher.o $(LDFLAGS)

clean:
	rm -f *.o
	rm -f starter
	rm -f reader
	rm -f greedy_reader
	rm -f writer
	rm -f spy
	rm -f finisher
	
