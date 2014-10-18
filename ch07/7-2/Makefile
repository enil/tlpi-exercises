CC			= gcc
CFLAGS		= -g -DDEBUG -D_BSD_SOURCE -std=c99
TARGET		= alloctest
OBJS		= alloctest.o newalloc.o
CLEANFILES = $(TARGET) $(OBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

alloctest.o: alloctest.c newalloc.h
newalloc.o: newalloc.c newalloc.h

