C			= gcc
CFLAGS		= -g -DDEBUG -D_GNU_SOURCE -std=gnu99
TARGET		= ptree
OBJS		= main.o plist.o ptree.o proc.o pchildren.o
CLEANFILES = $(TARGET) $(OBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

main.c: ptree.h pchildren.h
plist.o: plist.c plist.h proc.h
ptree.o: ptree.c ptree.h plist.h
proc.o: proc.c proc.h pchildren.h
pchildren.o: pchildren.c pchildren.h

