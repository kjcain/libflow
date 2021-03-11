all: clean build test

clean:
	scripts/clean.sh

build:
	scripts/build.sh

test:
	scripts/test.sh

.PHONY: all clean build test
