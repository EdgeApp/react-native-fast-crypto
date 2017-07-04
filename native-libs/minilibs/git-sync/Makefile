CFLAGS += -Wall -D_GNU_SOURCE -std=c99 -g -O2
LIBS := $(shell pkg-config --libs libgit2)

sync: build/main.o build/sync.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

libgit-sync.a: build/sync.o
	$(AR) rcs libgit-sync.a $^

build/test: build/test.o build/sync.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

test: build/test
	$(RM) -r test
	./build/test

clean:
	$(RM) -r build test
	$(RM) sync
	$(RM) libgit-sync.a

# Automatic dependency rules:
build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c -MMD $(CFLAGS) -o $@ $<

include $(shell find build -name *.d)
%.h: ;
