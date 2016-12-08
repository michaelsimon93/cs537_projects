================================================
      		README file
================================================
CS 537 - Operating Systems
Discussion Section - 302
Programming Assignment Number: 1    
Date Completed: 9/18/15

Name: Michael Simon
CS Login: msimon

The problems that I had with 1a and rangesort.c was with my error checking of the arguments, input/output files, and some of the range values. I tried adding some different checks but it ended up hurting my test results, so I left it as is. Besides that, the program works. It passed the contest test and sorts the records. I created a compare function at the top that I used when I called qsort. In main(), I checked the command line and handled the input, output, low value and high value. Then I opened the input file that was generated with generate.c and counted how many records fell within the range. I then used a pointer with malloc to hold the amount of records that fell within the range, opened the input file again, and then stored them. After this, I called qsort then iterated through the range to write the output file.

After running the test in 1b, I got 5/6. The only problem I came across was finding where to inrement the number of successful calls. I made changes in syscall.c, usys.S, sysfunc.h, user.h, and syscall.h.   
