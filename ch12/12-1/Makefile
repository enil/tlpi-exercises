C			= gcc
CFLAGS		= -g -DDEBUG -D_GNU_SOURCE -std=gnu99
TARGET		= plist
OBJS		= plist.o status.o proc.o dir.o misc.o error.o
CLEANFILES = $(TARGET) $(OBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

status.o: status.c status.h
proc.o: proc.c proc.h
dir.o: dir.c dir.h
misc.o: misc.c misc.h
error.o: error.c error.h

