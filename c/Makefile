DEBUG=-g -Wall#-pedantic
CFLAGS=-O0 #-arch x86_64
LFLAGS=#-lm -lpthread
CC=gcc

SRCDIR=../src
INCDIR=../inc

ANASRC=${SRCDIR}/anagram.c ${SRCDIR}/trie.c ${SRCDIR}/results.c 

ANAINC=${INCDIR}/anagram.h ${INCDIR}/trie.h ${INCDIR}/results.h

ANAOBJ=${ANASRC:.c=.o}



anagram: ${ANAOBJ}
	${CC} ${DEBUG} ${CFLAGS} ${ANAOBJ} ${LFLAGS} -o anagram

%.o: %.c ${ANAINC}
	${CC} ${DEBUG} ${CFLAGS} -I/usr/include -c -o $@ $<

all: anagram

clean: 
	rm anagram ${ANAOBJ}
	
