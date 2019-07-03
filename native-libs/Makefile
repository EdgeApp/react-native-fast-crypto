# Build settings:
WORK_DIR ?= build
INSTALL_PATH ?= /usr/local

# Set latest Git revision as Version
ifeq (,$(wildcard src/Version.h))
$(shell cp "src/Version.h.in" "src/Version.h")
endif

GIT_REV = $(shell git rev-parse --short HEAD)
SRC_REV = $(shell grep -o "\".*\"" src/Version.h)
GIT_REV_COMPARE = $(shell [ $(GIT_REV) !=  $(SRC_REV) ] && echo true)

ifeq ($(GIT_REV_COMPARE),true)
$(shell sed -i '/#define ABC_VERSION ".*"/c\#define ABC_VERSION "$(GIT_REV)"' src/Version.h )
endif

# Set DEFAULT_HIDDENBITSKEY in CLI if passed as variable
ifneq (,$(HIDDENBITSKEY))
$(shell sed -i '/#define DEFAULT_HIDDENBITSKEY ".*"/c\#define DEFAULT_HIDDENBITSKEY "$(HIDDENBITSKEY)"' cli/Main.cpp )
endif

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

# Source files:
abc_sources = \
	$(wildcard src/*.cpp)

cli_sources = $(wildcard cli/*.cpp cli/*/*.cpp)
test_sources = $(wildcard test/*.cpp)

# Objects:
abc_objects = $(addprefix $(WORK_DIR)/, $(addsuffix .o, $(basename $(abc_sources))))
cli_objects = $(addprefix $(WORK_DIR)/, $(addsuffix .o, $(basename $(cli_sources))))
test_objects = $(addprefix $(WORK_DIR)/, $(addsuffix .o, $(basename $(test_sources))))

# Adjustable verbosity:
V ?= 0
ifeq ($V,0)
	RUN = @echo Making $@...;
endif

# Targets:
all: $(WORK_DIR)/abc-cli check format-check
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

