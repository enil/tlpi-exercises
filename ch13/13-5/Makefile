C			= gcc
CFLAGS		= -g -DDEBUG -D_GNU_SOURCE -std=gnu99
TARGET		= tail
OBJS		= main.o opts.o block.o
CLEANFILES = $(TARGET) $(OBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

main.o: main.c opts.h block.h
opts.o: opts.c opts.h
block.o: block.c block.h

