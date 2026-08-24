CC = g++
CFLAGS = -Wall -Wextra -g
TARGETS = producer consumer
all: $(TARGETS)
producer: producer.o sem.o
	$(CC) $(CFLAGS) -o producer producer.o sem.o
consumer: consumer.o sem.o
	$(CC) $(CFLAGS) -o consumer consumer.o sem.o
producer.o: producer.c sem.h
	$(CC) $(CFLAGS) -c producer.c
consumer.o: consumer.c sem.h
	$(CC) $(CFLAGS) -c consumer.c
sem.o: sem.c sem.h
	$(CC) $(CFLAGS) -c sem.c
clean:
	rm -f *.o $(TARGETS)

