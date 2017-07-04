PREFIX ?= /usr/local
CFLAGS += -fPIC -O2

libscrypt.a: crypto_scrypt.o
	$(AR) rcs libscrypt.a $^

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

install: libscrypt.a
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include/scrypt
	cp libscrypt.a $(PREFIX)/lib/
	cp *.h $(PREFIX)/include/scrypt/

clean:
	$(RM) *.o *.a
