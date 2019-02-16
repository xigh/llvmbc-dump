DIRS=src tests

all: $(DIRS)

$(DIRS):
	$(MAKE) -C $@

clean:
	$(MAKE) -C src clean
	$(MAKE) -C tests clean
	rm -rf *.bc *.ll

.PHONY: all clean $(DIRS)
