CFLAGS = -std=gnu11 -Wall -Werror -Wextra
LDLIBS = -lrt -pthread

.PHONY: all clean

all: task1

clean:
	$(RM) task1

task1: task1.c