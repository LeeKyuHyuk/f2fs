SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: clean ext2

%.o: %.c
	gcc -c $< -o $@

ext2: $(OBJS)
	gcc -o f2fs $(OBJS) -lreadline

clean:
	rm f2fs *.o >/dev/null 2>/dev/null || true

run:
	./f2fs f2fs.img
