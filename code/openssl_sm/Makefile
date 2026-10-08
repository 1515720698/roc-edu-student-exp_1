CC = gcc
CFLAGS = -Wall -O2
LDLIBS = -lcrypto

sm_demo: sm_demo.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f sm_demo

.PHONY: clean
