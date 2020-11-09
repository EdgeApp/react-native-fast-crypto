# Build settings:
WORK_DIR ?= build
INSTALL_PATH ?= /usr/local

# Compiler options:
CFLAGS   += -D_GNU_SOURCE -DDEBUG -g -Wall -fPIC -std=c99
CXXFLAGS += -D_GNU_SOURCE -DDEBUG -g -Wall -fPIC -std=c++11
deps = libsecp256k1 libssl
LIBS := $(shell pkg-config --libs --static $(deps)) \
	-lmymonerocorecpp \
	-lboost_thread \
	-lboost_system \
	-lm

# Do not use -lpthread on Android:
ifneq (,$(findstring android,$(CC)))
	CFLAGS += -DANDROID
	CXXFLAGS += -DANDROID
	LIBS := $(filter-out -lpthread,$(LIBS)) -llog
endif

# Source files:
abc_sources = \
	$(wildcard src/*.cpp) \
	minilibs/scrypt/crypto_scrypt.c \
	minilibs/scrypt/sha256.c

# Objects:
abc_objects = $(addprefix $(WORK_DIR)/, $(addsuffix .o, $(basename $(abc_sources))))

# Adjustable verbosity:
V ?= 0
ifeq ($V,0)
	RUN = @echo Making $@...;
endif

# Targets:
all: libnativecrypto.a libnativecrypto.so
libnativecrypto.a:  $(WORK_DIR)/libnativecrypto.a
libnativecrypto.so: $(WORK_DIR)/libnativecrypto.so

$(WORK_DIR)/libnativecrypto.a: $(abc_objects)
	$(RUN) $(RM) $@; $(AR) rcs $@ $^

$(WORK_DIR)/libnativecrypto.so: $(abc_objects)
	$(RUN) $(CXX) -shared -Wl,-soname=libnativecrypto.so -o $@ $^ $(LDFLAGS) $(LIBS)

format:
	@astyle --options=astyle-options -Q --suffix=none --recursive --exclude=build --exclude=codegen --exclude=deps --exclude=minilibs "*.cpp" "*.hpp" "*.h"

format-check:
ifneq (, $(shell which astyle))
	@astyle --options=astyle-options -Q --suffix=none --recursive --exclude=build --exclude=codegen --exclude=deps --exclude=minilibs "*.cpp" "*.hpp" "*.h" \
	--dry-run | sed -n '/Formatted/s/Formatted/Needs formatting:/p'
endif

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

