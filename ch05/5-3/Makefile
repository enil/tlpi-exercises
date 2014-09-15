CC			= gcc
CFLAGS		= -g -std=c99
TARGET		= atomic_append
OBJS		= atomapp.o
CLEANFILES = $(TARGET) $(OBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

