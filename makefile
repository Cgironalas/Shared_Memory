CC=gcc

LDFLAGS= -lpthread -lm

starter: starter.o
	$(CC) starter.o -o starter $(LDFLAGS)
reader: reader.o
	$(CC) reader.o -o reader $(LDFLAGS)
selfish_reader: selfish_reader.o
	$(CC) selfish_reader.o -o selfish_reader $(LDFLAGS)
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
selfish_reader.o: selfish_reader.c
	$(CC) -c selfish_reader.c -o selfish_reader.o $(LDFLAGS)
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
	rm -f selfish_reader
	rm -f writer
	rm -f spy
	rm -f finisher
	
