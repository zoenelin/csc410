About: Now to Distributed Memory - MPI - Assignment 5 (A5)

Date: November 13, 2025
comments: files included are -> mainM.c matrixMul.h prime.c numIntegrate.c 

*** FIRST OPEN THE FOLDER WHERE THESE THREE FILES ARE LOCATED:
    In command prompt:
    cd folder-containing-the-files
    code .
***

Prime.c

To compile: mpicc prime.c -lm
To run: mpirun -np 4 ./a.out

To compile(2): mpicc numIntegrate.c -lm -o whatevernameyoulike
To run(2): mpirun ./whatevernamyoulike

(after -np enter the number of processes you wish for example 4, or leave blank to run as many processes as your computer can handle) 

*****************************************************************************************

numIntegrate.c or mainM.c/matrixMul.h

To compile: mpicc numIntegrate.c 
To run: mpirun -np 4 ./a.out

To compile (2): mpicc numIntegrate.c -o whatevernameyoulike
To run(2): mpirun ./whatevernamyoulike

(after -np enter the number of processes you wish for example 4, or leave blank to run as many processes as your computer can handle)


To compile mainM.c, just replace numIntegrate.c with any mainM.c program name to compile.

