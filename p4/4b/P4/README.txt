Authors:			
Benjamin Baeten	
cs:baeten 
wisc:bmbaeten

Michael Simon
cs: msimon
wisc: masimon2

Notes:

LINUX:
Changed the number of arguments to include the buffer size and number of threads. Added a Producer/Consumer function to create the thread pool and manage the buffer. Used locks and condition variables to provide concurrency with multiple threads. In the buffer, filled spaces with "-1" if there wasn't an active request. 

Failed "test locks". Not really sure why this is failing because "test locks2", "test locks3", and "test locks4" are all passing. 

Also failed "test equal", which I also couldn't figure out because "test equal2"and "test equal3" both pass.

Also failed "test fewer2" because it timed out/crashed.  


XV6:
created the appropriate functions and updated user.h, usys.S, syscall.c, sysfunc.h, etc, to sate the compiler.

for clone, much was copied from fork, except some more code was put in to copy the entire trap frame, and some code was copied from exec.c to modify the stack (using a buffer ustack[2])

join() was very much the same as wait() but with a few simple additions

added a condition in wait() to skip threads

added a condition in sbrk() to update thread sz

made a new library 'uthread.c' to hold the new lock functionality

