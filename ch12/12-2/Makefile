C			= gcc
CFLAGS		= -g -DDEBUG -D_GNU_SOURCE -std=gnu99
TARGET		= ptree
OBJS		= main.o ptree.o
CLEANFILES = $(TARGET) $(OBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

main.c: ptree.h
ptree.o: ptree.c ptree.h

