CC = gcc
CFLAGS = -Wall -O2
LDLIBS = -lcrypto

alice_send: alice_send.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f alice_send

.PHONY: clean
