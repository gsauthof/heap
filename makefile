
CXXFLAGSW_GCC = -Wall -Wextra -Wno-missing-field-initializers \
    -Wno-parentheses -Wno-missing-braces \
    -Wno-unused-local-typedefs \
    -Wfloat-equal \
    -Wpointer-arith -Wcast-align \
    -Wnull-dereference \
    -Wnon-virtual-dtor -Wmissing-declarations \
    -Werror=multichar -Werror=sizeof-pointer-memaccess -Werror=return-type \
    -Werror=delete-non-virtual-dtor \
    -fstrict-aliasing
#CXXFLAGSO = -Og -fsanitize=address -g
CXXFLAGSO = -O2
#CXXFLAGSO = -g

CXXFLAGS += $(CXXFLAGSW_GCC)
CXXFLAGS += $(CXXFLAGSO)

PYBIND11_CPPFLAGS := $(shell python3 -m pybind11 --includes)
PY_EXT_SUFFIX     := $(shell python3-config --extension-suffix)


.PHONY: all
all: heap$(PY_EXT_SUFFIX)


heap$(PY_EXT_SUFFIX): pyheap.cc heap.h heap.c heap.hh
	$(CXX) $(PYBIND11_CPPFLAGS) $(CXXFLAGS) -shared -fPIC $< -o $@

TEMP += heap$(PY_EXT_SUFFIX)

example/array_remove: example/array_remove.cc heap.hh heap.h heap.c
	$(CXX) -o $@ $(CPPFLAGS) $(CXXFLAGS) $<

.PHONY: check
check: heap$(PY_EXT_SUFFIX)
	python3 -m pytest test_heap.py -v


.PHONY: clean
clean:
	rm -rf $(TEMP)
