CC			= gcc
CFLAGS		= -g
TARGET		= envtest
OBJS		= envtest.o env.o
CLEANFILES = $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

