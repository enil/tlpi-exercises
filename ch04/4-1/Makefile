CC			= gcc
TARGET		= tee
OBJS		= tee.o opts.o
CLEANFILES = $(TARGET) $(OBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

opts.o: opts.c opts.h

