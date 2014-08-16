##
## Makefile for matasano
## by lenormf
##

CC = gcc
CFLAGS = -Wall -Wextra -g -O0

all: hex_to_base64 fixed_xor single_byte_xor_cipher

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

hex_to_base64: hex_to_base64.o utils.o error.o
fixed_xor: fixed_xor.o utils.o error.o
single_byte_xor_cipher: single_byte_xor_cipher.o utils.o error.o
	$(CC) -o $@ $^

clean:
	rm -f $(wildcard *.o)

distclean: clean
	rm -f hex_to_base64

re: distclean all
