CC			= gcc
CFLAGS		= -g -std=c99
TARGET		= newduptest
OBJS		= newduptest.o newdup.o
CLEANFILES = $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

