CC = gcc
CFLAGS = -I$(HOME)/.local/gmssl/include
LDFLAGS = -L$(HOME)/.local/gmssl/lib -lgmssl

all: gmssl_demo

gmssl_demo: gmssl_demo.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

run: gmssl_demo
	LD_LIBRARY_PATH=$(HOME)/.local/gmssl/lib ./gmssl_demo

clean:
	rm -f gmssl_demo

.PHONY: all run clean
