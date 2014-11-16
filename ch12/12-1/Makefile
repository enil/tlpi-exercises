C			= gcc
CFLAGS		= -g -DDEBUG -std=gnu99
TARGET		= plist
OBJS		= plist.o
CLEANFILES = $(TARGET) $(OBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

