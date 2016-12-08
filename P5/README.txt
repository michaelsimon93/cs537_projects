Authors:			
Benjamin Baeten	
cs:baeten 
wisc:bmbaeten

Michael Simon
cs: msimon
wisc: masimon2

Notes:

LINUX:


XV6:
 I created the appropriate MAPs for the addresses to split them up into address and checksum parts.

 Also created an expression to get the 2 most significant bytes from an address and convert it into a 2 byte checksum.

I followed the structure of bmap to go through each block during readi and writei and perform a checksum operation, and compared it with the disk checksum when appropriate

also updated stati.
