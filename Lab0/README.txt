This program contains below parts:
- Text generator, generating 10k-symbol text with given source.  
- Encoding generated text, using certain algorithm, while calculating coding price
- Decoding text, evaluating data compression ratio
- Checking decoded text

- Evaluate effectiveness among algorithm

Algorithms used:
- Fano
- RLE
- Hybrid: RLE + Fano + RLE 

//-------------------------------------------

process:

//note
General decoding doesn't work with RLE bc it reads file by char. 
To handle that case, we should add an index to announce the function when decoded file
is RLE-encoded. 
or read till 255 (max char)

Fano-decoding: first read the whole file to bitunpack. 

// About encoded file
Must be open in BINARY mode, to avoid data loss or miswrite. 



