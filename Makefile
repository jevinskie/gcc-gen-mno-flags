TARGETS := test-load-gcc

C_CXX_FLAGS := -Wall -Wextra -Wpedantic
CXX_FLAGS := $(C_CXX_FLAGS) -std=gnu++23

all: $(TARGETS)

.PHONY: clean

clean:
	rm -rf *.dSYM/
	rm -f $(TARGETS)

test-load-gcc: test-load-gcc.cpp
	$(CXX) -o $@ $^ $(CXX_FLAGS) -I$(HOME)/base/gcc-15/lib/gcc/x86_64-pc-linux-gnu/15.0.0/plugin/include
