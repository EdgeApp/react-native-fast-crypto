.PHONY: check clean

# Build settings:
WORK_DIR ?= build
INSTALL_PATH ?= /usr/local
PREFIX ?= deps/build/prefix

# Compiler options:
CFLAGS   += -D_GNU_SOURCE -DDEBUG -g -Wall -fPIC -std=c99
CXXFLAGS += -D_GNU_SOURCE -DDEBUG -g -Wall -fPIC -std=c++11
LIBS := -lmymonerocorecpp \
	-lboost_thread \
	-lboost_system \
	-lm

# Do not use -lpthread on Android:
ifneq (,$(findstring android,$(CC)))
	CFLAGS += -DANDROID
	CXXFLAGS += -DANDROID
	LIBS := $(filter-out -lpthread,$(LIBS)) -llog
endif

# Test-only setup
T ?= 0
ifeq ($T, 1)
	CXXFLAGS += -I$(PREFIX)/native/include
	LDFLAGS += -L$(PREFIX)/native/lib
endif

# Source files:
abc_sources = $(wildcard src/*.cpp)
test_sources = $(wildcard test/*.cpp)

# Objects:
abc_objects = $(addprefix $(WORK_DIR)/, $(addsuffix .o, $(basename $(abc_sources))))
test_objects = $(addprefix $(WORK_DIR)/, $(addsuffix .o, $(basename $(test_sources))))

# Adjustable verbosity:
V ?= 0
ifeq ($V,0)
	RUN = @echo Making $@...;
endif

# Targets:
libnativecrypto.a:  $(WORK_DIR)/libnativecrypto.a
libnativecrypto.so: $(WORK_DIR)/libnativecrypto.so

$(WORK_DIR)/libnativecrypto.a: $(abc_objects)
	$(RUN) $(RM) $@; $(AR) rcs $@ $^

$(WORK_DIR)/libnativecrypto.so: $(abc_objects)
	$(RUN) $(CXX) -shared -Wl,-soname=libnativecrypto.so -o $@ $^ $(LDFLAGS) $(LIBS)

$(WORK_DIR)/abc-test: $(test_objects)
	$(RUN) $(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

check: $(WORK_DIR)/abc-test
	$(RUN) $<

clean:
	$(RM) -r $(WORK_DIR)

# Automatic dependency rules:
$(WORK_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(RUN) $(CC) -c -MD $(CFLAGS) -o $@ $<

$(WORK_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(RUN) $(CXX) -c -MD $(CXXFLAGS) -o $@ $<

include $(wildcard $(WORK_DIR)/*/*.d $(WORK_DIR)/*/*/*.d $(WORK_DIR)/*/*/*/*.d)
%.h: ;
%.hpp: ;

