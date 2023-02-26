This program contains below parts:
- Text generator, generating 10k-symbol text with given source.  
- Encoding generated text, using certain algorithm, while calculating coding price
- Decoding text, evaluating data compression ratio
- Checking decoded text

- Evaluate effectiveness among algorithm

Algorithms used:
- Huffmans
- RLE
- Hybrid: RLE + Fano + RLE 

//-------------------------------------------

process:

//note
Decoding function doesn't work with RLE bc it reads file by char. 
To handle that case, we should add an index to announce the function when decoded file
is RLE-encoded. 
or read till 255 (max char)


Decoding function needs to read the whole file as a string (bc the file is written by BITS),
then look up bitword in dictionary to decode. 

// About encoded file
Must be open in BINARY mode, to avoid data loss or miswrite. 

//RLE update
Add a flag bitset to the end of file, with length = size of encoding file
Each bit = 0 if there is only ONE character at that position, and 0 if otherwise e.g. >=1. 

