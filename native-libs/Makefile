# Build settings:
WORK_DIR ?= build
INSTALL_PATH ?= /usr/local

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

# Objects:
abc_objects = $(addprefix $(WORK_DIR)/, $(addsuffix .o, $(basename $(abc_sources))))

# Adjustable verbosity:
V ?= 0
ifeq ($V,0)
	RUN = @echo Making $@...;
endif

# Targets:
all: $(WORK_DIR)/abc-cli check
libnativecrypto.a:  $(WORK_DIR)/libnativecrypto.a
libnativecrypto.so: $(WORK_DIR)/libnativecrypto.so

$(WORK_DIR)/libnativecrypto.a: $(abc_objects)
	$(RUN) $(RM) $@; $(AR) rcs $@ $^

$(WORK_DIR)/libnativecrypto.so: $(abc_objects)
	$(RUN) $(CXX) -shared -Wl,-soname=libnativecrypto.so -o $@ $^ $(LDFLAGS) $(LIBS)

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

