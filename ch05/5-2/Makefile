CC			= gcc
CFLAGS		= -g
TARGET		= appendstart
OBJS		= appendstart.o
CLEANFILES = $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

