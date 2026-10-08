CC = gcc
CFLAGS = -Wall -O2
LDLIBS = -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lgmssl

gm_demo: gm_demo.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f gm_demo

.PHONY: clean
