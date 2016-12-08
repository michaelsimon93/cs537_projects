Authors:			
Benjamin Baeten	
cs:baeten 
wisc:bmbaeten

Michael Simon
cs: msimon
wisc: masimon2

Notes:
We implemented the 4 queues by splitting the original array into four parts,
and using pointer arithmatic to find out the priority of each process by which section it was in

We implemented a way to use getpinfo to fill the struct as described on the webpage,

however, when we tried to compile with the tests, we received the error:
kernel/pstat.h:4: *** missing separator.  Stop.

We could not figure out how to fix this.
We copied the pstat.h file from the webpage, and tried changing the spaces to tabs, but nothing seemed to work

We also ran out of time to create a graph, and will have to place it in the late directory
