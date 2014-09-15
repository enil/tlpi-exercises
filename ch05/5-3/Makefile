CC			= gcc
CFLAGS		= -g -std=c99
TARGET		= atomic_append
OBJS		= atomapp.o
CLEANFILES = $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

