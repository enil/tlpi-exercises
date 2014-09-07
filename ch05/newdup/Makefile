CC			= gcc
CFLAGS		= -g
TARGET		= newduptest
OBJS		= newduptest.o newdup.o
CLEANFILES = $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

