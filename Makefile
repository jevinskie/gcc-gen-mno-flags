TARGETS := test-load-gcc hello-world-gcc-plugin.so

C_CXX_FLAGS := -Wall -Wextra -Wpedantic
CXX_FLAGS := $(C_CXX_FLAGS) -std=gnu++23 -ggdb3

GCC := gcc-15
GCC_PLUGIN_BASE := $(shell $(GCC) -print-file-name=plugin)
GCC_PLUGIN_CXX_FLAGS := -I $(GCC_PLUGIN_BASE)/include

all: $(TARGETS)

.PHONY: clean

clean:
	rm -rf *.dSYM/
	rm -f $(TARGETS)

test-load-gcc: test-load-gcc.cpp
	$(CXX) -o $@ $^ $(CXX_FLAGS) $(GCC_PLUGIN_CXX_FLAGS)

hello-world-gcc-plugin.so: hello-world-gcc-plugin.cpp
	$(CXX) -o $@ $^ -shared -fPIC $(CXX_FLAGS) $(GCC_PLUGIN_CXX_FLAGS)
