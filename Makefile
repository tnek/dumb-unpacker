CC=gcc
CFLAGS=-Wall -fPIC

libfake.o: 
	$(CC) $(CFLAGS) -c -o libfake.o fake.c

libfake.so:
	$(CC) $(CFLAGS) -shared -Wl,-soname -Wl,libfake.so -o libfake.so libfake.o -ldl

clean:
	rm libfake.*
