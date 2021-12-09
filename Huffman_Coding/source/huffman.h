/*
* Huffman codes
*
* NOTE: THIS FILE IS AUTO GENERATED -- Do not edit
*/
#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <stdint.h>

typedef struct
{
    unsigned char symbol;      // Symbol being coded
    uint32_t code;             // Binary code for symbol
    int code_bits;             // Number of bits in code
}huffman_code_t;

#define HUFF_CODE_END_SYMBOL  '\0'
#define HUFF_CODE_MIN_LENGTH (4)
#define HUFF_CODE_MAX_LENGTH (19)

huffman_code_t huffman_code[] = {
{' ',0x4,3},
{'e',0xb,4},
{'0',0x4,4},
{':',0x1e,5},
{'1',0x1a,5},
{'s',0x18,5},
{'i',0x15,5},
{'t',0x14,5},
{'o',0xc,5},
{'n',0xb,5},
{'d',0xa,5},
{'2',0x6,5},
{'r',0x5,5},
{'a',0x2,5},
{'h',0x3f,6},
{'3',0x3a,6},
{'l',0x37,6},
{'.',0x36,6},
{'k',0x32,6},
{'7',0x1e,6},
{'u',0x1d,6},
{'5',0x1c,6},
{'c',0x1b,6},
{'4',0xf,6},
{'9',0x9,6},
{'v',0x8,6},
{'6',0x7,6},
{'m',0x6,6},
{'8',0x2,6},
{'p',0x1,6},
{'-',0x0,6},
{'f',0x7d,7},
{']',0x77,7},
{'[',0x76,7},
{'g',0x73,7},
{'\n',0x72,7},
{'b',0x71,7},
{'N',0x70,7},
{'/',0x3e,7},
{'x',0xf8,8},
{'y',0xce,8},
{'I',0xcc,8},
{'P',0x7e,8},
{'S',0x6b,8},
{'(',0x69,8},
{'A',0x3b,8},
{'C',0x3a,8},
{')',0x39,8},
{'R',0xd,8},
{'_',0xc,8},
{'w',0x1f3,9},
{'M',0x1f2,9},
{'E',0x19f,9},
{'=',0x19e,9},
{'T',0x19b,9},
{'D',0xff,9},
{'"',0xfe,9},
{'L',0xd5,9},
{'O',0xd4,9},
{'B',0xd0,9},
{'U',0x71,9},
{'F',0x70,9},
{',',0x1c,9},
{'\'',0x1a2,10},
{'G',0x3e,10},
{'X',0x3d,10},
{'W',0x3c,10},
{'H',0x3b,10},
{'z',0x66a,11},
{'V',0x668,11},
{'%',0x7f,11},
{'K',0x7e,11},
{'>',0x75,11},
{'+',0xcd7,12},
{'q',0xcd2,12},
{'*',0x68f,12},
{'Y',0x68e,12},
{'j',0x68d,12},
{'Q',0xe9,12},
{'~',0xe8,12},
{'<',0x19ad,13},
{'#',0x19ac,13},
{'J',0x19a7,13},
{'Z',0x1a33,14},
{'\\',0x1a32,14},
{'@',0x1a31,14},
{'&',0x669b,15},
{';',0x6699,15},
{'?',0x6698,15},
{'!',0x3461,15},
{'^',0x3460,15},
{'|',0xcd34,16},
{'\t',0x19a6a,17},
{'}',0x669af,19},
{'`',0x669ae,19},
{'{',0x669ad,19},
{'$',0x669ac,19},
{HUFF_CODE_END_SYMBOL,0,0}};


/*
* @brief - Encodes a message using the defined Huffman code.
*
* Parameters:
* message   - Null terminated mesage to be encoded
* buffer    - Resulting encoded bytes will be encoded huffman_code_tree
* nbytes    - Size of buffer, in nbytes
*
* Returns:
*   Number of bytes encoded
*/
int huffman_encode(char *message, uint8_t *buffer, size_t nbytes);


#endif /*HUFFMAN_H_*/
