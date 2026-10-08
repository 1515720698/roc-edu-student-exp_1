CC = gcc
GMSSL_PREFIX = $(HOME)/.local/gmssl
CFLAGS = -I$(GMSSL_PREFIX)/include
LDFLAGS = -L$(GMSSL_PREFIX)/lib -lgmssl

all: gmssl_env_send gmssl_env_recv

gmssl_env_send: gmssl_env_send.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

gmssl_env_recv: gmssl_env_recv.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

test: all
	LD_LIBRARY_PATH=$(GMSSL_PREFIX)/lib ./test.sh

clean:
	rm -f gmssl_env_send gmssl_env_recv

.PHONY: all clean test
