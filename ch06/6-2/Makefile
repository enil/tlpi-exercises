CC			= gcc
CFLAGS		= -g
TARGET		= badlongjmp
OBJS		= badlongjmp.o
CLEANFILES = $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

