include prorab.mk

this_name := tests

this_cxxflags += -Wall
this_cxxflags += -Wno-format #no warnings about format
this_cxxflags += -Wno-format-security #no warnings about format
this_cxxflags += -DDEBUG
this_cxxflags += -fstrict-aliasing #strict aliasing!!!
this_cxxflags += -std=c++11
this_cxxflags += -O3



this_srcs += main.cpp

this_ldlibs += -lstob -lpapki -lstdc++
this_ldflags += -L$(prorab_this_dir)../../src

ifeq ($(prorab_os), macosx)
    this_cxxflags += -stdlib=libc++ #this is needed to be able to use c++11 std lib
    this_ldlibs += -lc++
else ifeq ($(prorab_os),windows)
    this_cxxflags += -I/usr/include -I/usr/local/include
    this_ldflags += -L/usr/lib -L/usr/local/lib
endif


$(eval $(prorab-build-app))


ifeq ($(prorab_os),windows)
    this_test_cmd := $(prorab_echo)(cd $(prorab_this_dir); cp ../../src/libstob.dll .; $$^)
else
    this_test_cmd := $(prorab_echo)(cd $(prorab_this_dir); LD_LIBRARY_PATH=../../src $$^)
endif


define this_rules
test:: $(prorab_this_name)
	@echo running $$^...
	$(this_test_cmd)
endef
$(eval $(this_rules))

#add dependency on libstob
$(prorab_this_name): $(abspath $(prorab_this_dir)../../src/libstob$(prorab_lib_extension))

$(eval $(call prorab-include,$(prorab_this_dir)../../src/makefile))
