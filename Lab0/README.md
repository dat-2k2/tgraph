
#ENCODE/DECODE

#RLE

*Run-length encoding*. \
character series -> character + occurrences.

#Fano

...

#Bitpacking

Mapping bytes to bits, data copied by modified empty string. A flag is added to the end to avoid skipping ending-empty character, simultaneously record number of padding bits. 

#Unpacking

First read the flag bit, then extract the data into string of 0 1. Finally pop padding bits. 
