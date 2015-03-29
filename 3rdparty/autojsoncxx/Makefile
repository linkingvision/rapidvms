CXX ?= c++
CXXFLAGS ?= -std=c++0x -O2 -Wall -Wextra -pedantic -g

export

all:
	$(MAKE) -C test/

test:
	$(MAKE) test -C test/

clean:
	$(MAKE) clean -C test/

.PHONY: test clean
