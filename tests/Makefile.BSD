DIRS=lib src

all: $(DIRS)

$(DIRS):
	$(MAKE) -C $@

clean:
	$(MAKE) -C lib clean
	$(MAKE) -C src clean

.PHONY: all clean $(DIRS)
