CC			= gcc
CFLAGS		= -g
TARGET		= badlongjmp
OBJS		= badlongjmp.o
CLEANFILES = $(TARGET) $(OBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

