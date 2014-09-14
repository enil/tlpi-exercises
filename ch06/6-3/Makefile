CC			= gcc
CFLAGS		= -g -std=c99 -D_GNU_SOURCE
TARGET		= envtest
OBJS		= envtest.o env.o
CLEANFILES = $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

