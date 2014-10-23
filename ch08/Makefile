CC			= gcc
CFLAGS		= -g -DDEBUG -D_BSD_SOURCE -std=c99
TARGET		= pwdtest
OBJS		= pwdtest.o newpwd.o
CLEANFILES = $(TARGET) $(OBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK.o) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(CLEANFILES)

pwdtest.o: pwdtest.c newpwd.h
newpwd.o: newpwd.c newpwd.h

