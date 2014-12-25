C			= gcc
CFLAGS		= -g -DDEBUG -D_GNU_SOURCE -std=gnu99
TARGET		= psfile
OBJS		= main.o psfile.o
CLEANFILES = $(TARGET) $(OBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

main.o: main.c psfile.h
psfile.o: psfile.c psfile.h

