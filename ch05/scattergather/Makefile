CC			= gcc
CFLAGS		= -g
TARGET		= sgtest
OBJS		= sgtest.o sg.o
CLEANFILES = $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

sgtest.o: sgtest.c sg.h

