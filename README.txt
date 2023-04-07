A C program to determine how many page faults will occur with a FIFO replacement policy.
Input from the command line will be read with 2 parameters as shown below:

myprogram pagereffile memorysize

The file will have an integer set of page references separated by spaces without any newlines of space at the end of the line.
The number of page references is not limited. The memorysize must be between 1 and 10.
The program will output the number of page faults and the final state of memory.

The sample reference string will be used in testing the program:

7 0 1 2 0 3 0 4 2 3 0 3 0 3 2 1 2 0 1 7 0 1