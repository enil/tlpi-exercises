CC			= gcc
CFLAGS		= -g -DDEBUG -std=gnu99
TARGET		= grptest
OBJS		= grptest.o newgrp.o
CLEANFILES = $(TARGET) $(OBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

grptest.o: grptest.c newgrp.h
newgrp.o: newgrp.c newgrp.h

