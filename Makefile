all: install-dependencies clean build test

clean:
	scripts/clean.sh

build:
	scripts/build.sh

test:
	scripts/test.sh

.flag-installed-dependencies:
	scripts/install_dependencies.sh
	touch .flag-installed-dependencies

install-dependencies: .flag-installed-dependencies

.PHONY: all clean build test install-dependencies
