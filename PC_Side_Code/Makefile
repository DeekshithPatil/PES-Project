all:	main.c	serial.c huffman.h
	gcc main.c serial.c -o main


huffman.h: huffman.py
	python3 huffman.py > huffman.h
