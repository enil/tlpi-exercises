SUBDIRS = $(wildcard ch*/*-*/)

.PHONY: all clean

all clean: $(SUBDIRS)
	@for dir in $^; do $(MAKE) -C $$dir $@; done


