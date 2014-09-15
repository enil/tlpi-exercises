CC			= gcc
CFLAGS		= -g -std=c99 -D_BSD_SOURCE
TARGET		= dupoffs
OBJS		= dupoffs.o
CLEANFILES = $(TARGET) $(OBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

