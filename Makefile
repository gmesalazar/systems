SUBDIRS := uar ucc ued upt ush uws

all clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@;  \
	done

.PHONY: all $(SUBDIRS)
