C			= gcc
CFLAGS		= -g -DDEBUG -D_GNU_SOURCE -std=gnu99
TARGET		= tail
OBJS		= main.o
CLEANFILES = $(TARGET) $(OBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

