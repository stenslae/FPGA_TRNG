# Compiler and flags
CC = gcc

CFLAGS = -Wall -g
OPENSSL = -lssl -lcrypto
# Final executables
all: encrypt

# Compile source to object files
encrypt: encrypt_sw.o tools.o
	$(CC) $(CFLAGS) encrypt_sw.o tools.o -o server $(OPENSSL)

# Compile source to object files
encrypt_sw.o: encrypt_sw.c tools.h
	$(CC) $(CFLAGS) -c encrypt_sw.c

tools.o: tools.c tools.h
	$(CC) $(CFLAGS) -c tools.c

# Clean up compiled files
clean:
	rm -f *.o