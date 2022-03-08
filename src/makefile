OBJS = logger.o main.o redis.o
SOURCE = logger.c main.c redis.c
HEADER = colors.h logger.h redis.h
OUT	= run.out
CC = gcc
FLAGS = -g -c -Wall
LFLAGS = -lhiredis

all: $(OBJS)
	@echo "Finishing..."
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)
	mv *.o bin/
	mv *.out exec/
	@echo "Job done."

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