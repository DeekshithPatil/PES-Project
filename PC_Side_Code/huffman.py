# Huffman Coding in python
#reference: https://www.section.io/engineering-education/huffman-coding-python/

string = ""
f = open("logFiles.txt", "r")

data = f.read()

string = str(data)

# Creating tree nodes
class NodeTree(object):

    def __init__(self, left=None, right=None):
        self.left = left
        self.right = right

    def children(self):
        return (self.left, self.right)

    def nodes(self):
        return (self.left, self.right)

    def __str__(self):
        return '%s_%s' % (self.left, self.right)


# Main function implementing huffman coding
def huffman_code_tree(node, left=True, binString=''):
    if type(node) is str:
        return {node: binString}
    (l, r) = node.children()
    d = dict()
    d.update(huffman_code_tree(l, True, binString + '0'))
    d.update(huffman_code_tree(r, False, binString + '1'))
    return d


# Calculating frequency
freq = {}
for c in string:
    if c in freq:
        freq[c] += 1
    else:
        freq[c] = 1

freq = sorted(freq.items(), key=lambda x: x[1], reverse=True)

nodes = freq

while len(nodes) > 1:
    (key1, c1) = nodes[-1]
    (key2, c2) = nodes[-2]
    nodes = nodes[:-2]
    node = NodeTree(key1, key2)
    nodes.append((node, c1 + c2))

    nodes = sorted(nodes, key=lambda x: x[1], reverse=True)

huffmanCode = huffman_code_tree(nodes[0][0])

# Print Header file
c_hdr = """\
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

#define HUFF_CODE_END_SYMBOL  '\\0'"""

print(c_hdr)

minimum = 65535
maximum = 0

for (char, frequency) in freq:
    length = len(huffmanCode[char])
    if(length > maximum):
        maximum = length
    if(length < minimum):
        minimum = length

print(f'#define HUFF_CODE_MIN_LENGTH ({minimum})')
print(f'#define HUFF_CODE_MAX_LENGTH ({maximum})')
print("")
print("huffman_code_t huffman_code[] = {")

hexNumber = 0
count = 0

# Print characters, code and code_bits

for (char, frequency) in freq:



    hexNumber = hex(int((huffmanCode[char]),2))

    if(char == "'"):
        print("{'\\'',%s,%d}," % (hexNumber,len(huffmanCode[char])))
    else:

        print('{%r,%s,%d},' % (char, hexNumber,len(huffmanCode[char])))



    count = count + 1

print("{HUFF_CODE_END_SYMBOL,0,0}};")

print("")

# Print function description
c_func_desc = """
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
"""

print(c_func_desc)

print("")

print("#endif /*HUFFMAN_H_*/")
