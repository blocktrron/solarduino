ifndef MAC_ADDRESS
MAC_ADDRESS := $(shell python3 $(CURDIR)/tools/random_mac.py)
endif

all:
	@echo "Building with MAC-Address: $(MAC_ADDRESS)"
	@export PLATFORMIO_BUILD_FLAGS='-DMAC_ADDRESS=\"$(MAC_ADDRESS)\"' && platformio run $(RUN_OPTIONS)

upload: RUN_OPTIONS=--target upload
upload: all

clean:
	@rm -rf $(CURDIR)/.pioenvs
	@rm -rf $(CURDIR)/.piolibdeps

.PHONY : clean