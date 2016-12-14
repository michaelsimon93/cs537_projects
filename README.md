# cs537_projects
These projects were for my CS537 - Operating Systems course in the Fall of 2015 at the University of Wisconsin-Madison. 
All of these projects were programmed in C. Below are the descriptions for each project:

p1a:
Build a sorting program called rangesort that takes in one of these generated files and sorts it based on the 4-byte key (the remainder of the record should of course be kept with the same key). Keys which fall outside of lowvalue and highvalue should be discarded (and not written to the output file). The output is written to the specified output file.

p1b:
The goal of the project is simple: to add two system calls to xv6. The system calls are:
  getsyscallnumtotal() returns the total number of system calls that have been issued.
  getsyscallnumsuccess() returns the number of system calls that have completed successfully (i.e., did not have a return value of -1).
Each system call will simply return the value of one of two counters; you will need to add both of these new counters. One of the counters will be incremented before any system call is issued (this counter is returned by getsyscallnumtotal()). The other counter will be incremented after the system call has returned, if and only if the system call did not return a -1 (this counter is returned by getsyscallnumsuccess()).

p2a: 
In this assignment, you will implement a command line interpreter or, as it is more commonly known, a shell. The shell should operate in this basic way: when you type in a command (in response to its prompt), the shell creates a child process that executes the command you entered and then prompts for more user input when it has finished.

p2b:
In this project, you'll be implementing a simplified multi-level feedback queue (MLFQ) scheduler in xv6. The basic idea is simple. Build an MLFQ scheduler with four priority queues; the top queue (numbered 0) has the highest priority and the bottom queue (numbered 3) has the lowest priority. When a process uses up its time-slice, it should be downgraded to the next (lower) priority level. The time-slices for higher priorities will be shorter than lower priorities.

p3:
In this project, you'll be changing xv6 to support a few features virtually every modern OS does. The first is causing an exception to occur when your program dereferences a null pointer; the second is rearranging the address space so as to place the stack at the high end.

p4a:
In this project, you will be adding one key piece of functionality to the basic web server: you will make it multi-threaded. You will also be modifying how the web server is invoked so that it can handle new input parameters (e.g., the number of threads to create). The basic web server that we provided has a single thread of control. Single-threaded web servers suffer from a fundamental performance problem in that only a single HTTP request can be serviced at a time. Thus, every other client that is accessing this web server must wait until the current http request has finished; this is especially a problem if the current http request is a long-running CGI program or is resident only on disk (i.e., is not in memory). Thus, the most important extension that you will be adding is to make the basic web server multi-threaded.

p4b:
In this project, you'll be adding real kernel threads to xv6. Specifically, you'll do three things. First, you'll define a new system call to create a kernel thread, called clone() , as well as one to wait for a thread called join() . Then, you'll use clone() to build a little thread library, with a thread_create() , lock_acquire() , and lock_release().

p5: 
In this project, you'll be changing the existing xv6 file system to add protection from data corruption. In real storage systems, silent corruption of data is a major concern, and thus many techniques are usually put in place to detect (and recover) from blocks that go bad. Specifically, you'll do three things. First, you'll modify the code to allow the user to create a new type of file that keeps a checksum for every block it points to. Checksums are used by modern storage systems in order to detect silent corruption. Second, you'll have to change the file system to handle reads and writes differently for files with checksums. Specifically, when writing out such a file, you'll have to create a checksum for every block of the file; when reading such a file, you'll have to check and make sure the block still matches the stored checksum, returning an error code (-1) if it doesn't. In this way, your file system will be able to detect corruption! Third, for information purposes, you will also modify the stat() system call to dump some information about the file. Thus, you should write a little program that, given a file name, not only prints out the file's size, etc., but also some information about the file's checksums (details below).
