OBJS = logger.o main.o redis.o sha-256.o base-32.o file.o tnfs.o
SOURCE = logger.c main.c redis.c sha-256.c base-32.c file.c tnfs.c
HEADER = colors.h logger.h redis.h sha-256.h base-32.h file.h tnfs.h
OUT	= tnfs
CC = gcc
FLAGS = -g -c -Wall
LFLAGS = -lhiredis

all: $(OBJS)
	@echo "Finishing..."
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)
	mv *.o bin/
	@echo "Job done."

tnfs.o: tnfs.c
	@echo "Building $<..."
	$(CC) $(FLAGS) $<

file.o: file.c
	@echo "Building $<..."
	$(CC) $(FLAGS) $<

sha-256.o: sha-256.c
	@echo "Building $<..."
	$(CC) $(FLAGS) $<

base-32.o: base-32.c
	@echo "Building $<..."
	$(CC) $(FLAGS) $<

logger.o: logger.c
	@echo "Building $<..."
	$(CC) $(FLAGS) $<

redis.o: redis.c
	@echo "Building $<..."
	$(CC) $(FLAGS) $<

main.o: main.c
	@echo "Building $<..."
	$(CC) $(FLAGS) $<

clean:
	rm -f bin/* exec/*