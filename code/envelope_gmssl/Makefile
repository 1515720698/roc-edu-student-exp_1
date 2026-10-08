CC = gcc
CFLAGS = -Wall -O2
LDLIBS = -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lgmssl

alice_recv_gm: alice_recv_gm.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f alice_recv_gm

.PHONY: clean
