Authors:			
Benjamin Baeten	
cs:baeten 
wisc:bmbaeten

Michael Simon
cs: msimon
wisc: masimon2

Notes:
We allocated a completely null page at the 0 end of our user space, then moved the stack to the high end, and adjusted some checks in vm.c, and syscall.c to account for the new stack location.

For the tests, we either implemented the designated error cases, or allocated large character arrays on the stack or heap to show that both could grow correctly.

Problems:
Our finished kernel does not seem to have any exisiting problems.

