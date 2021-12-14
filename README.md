# PES-Project

The aim of the project is to implement compression to speed debug prints over UART. Prior to
transmitting a particular string over the serial line, the KL25Z will employ Huffman coding to
compress debug messages prior to transmitting them over the UART. On the PC side, the complementary code is used to decompress and display the debug prints.

Refer Project - Proposal Document for more details.

# Implementation Details and Steps To Execute Program

## Huffman Codes Lookup Table:
A corpus of data is collected from system log files on Ubuntu, which is about 3 MB in size. A python script "huffman.py" reads this file and generate the huffman codes lookup table "huffman.h". This header file is included in both PC Side and KL25Z side. 

Note: Any changes made to huffman.py on one side, need to made on the other side as well. 

## KL25Z:
The project is compiled using MCUXpresso IDE. Modifications are made in make file, in order generate the "huffman.h" file which contains the code bits lookup table. The code is flashed into KL25Z with debug build configuration. 

It is recommend to terminate the debug session, and restart the KL25Z by pressing the reset button on board. This is done because, if the IDE is connected to the board, the board is slowed down. 

Note: This project does not have a release build. 

## PC Side:
The code for the PC side is in the folder "PC_Side_Code". Steps to execute, (Works only in Linux)

1. Open a terminal and navigate to the "PC_Side_Code" directory. 
2. Build the project by typing "make" in the terminal. 
3. Execute the project by typing: "./main <path to serial port>" e.g: ./main /dev/ttyACM0

## System (PC) details
Operating System: Ubuntu 20.04.2 LTS \
Kernel: Linux 5.11.0-40-generic \
Architecture: x86-64 
