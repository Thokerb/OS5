CFLAGS = -std=gnu11 -Wall -Werror -Wextra
LDLIBS = -lrt -pthread

.PHONY: all clean

all: task1 task2

clean:
	$(RM) task1 task2

task1: task1.c
task2: task2.c