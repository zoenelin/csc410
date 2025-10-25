About: Still about synchronization of threads - Assignment 3 part 2 (A3_2)

Date: October 24, 2025
comments: files included are -> Main_merge.c p_bubble_1.c p_bubble_2.c p_integrate.c p_integrate_sequential.c p_merge.c Recursive-thread-creation.c segment-based.c Thread-pool.c

*** FIRST OPEN THE FOLDER WHERE THESE THREE FILES ARE LOCATED:
    In command prompt:
    cd folder-containing-the-files
    code .
***

To compile: g++ p_integrate.c -lpthread
To run: ./a.out

OR

To compile: g++ p_integrate.c -lpthread -o whatevernameyoulike
To run: ./whatevernamyoulike

To compile any of the other program, just replace p_integrate.c with any other program name compile.

To compile Coding Task 2 approach 1: g++ main_merge.c segment-based.c -lpthread
To compile Coding Task 2 approach 2: g++ main_merge.c Recursive-thread-creation.c -lpthread
To compile Coding Task 2 approach 3: g++ main_merge.c Thread-pool.c -lpthread