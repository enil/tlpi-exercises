CC			= gcc
CFLAGS		= -g -std=c99
OBJS		= *.o
CLEANFILES	= writetest readtest $(OBJS)

all: writetest readtest

writetest: writetest.o sg.o opts.o common.o
	$(LINK.o) -o $@ $^

readtest: readtest.o sg.o opts.o common.o
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

writetest.o: writetest.c sg.h opts.h common.h
readtest.o: readtest.c sg.h opts.h common.h

