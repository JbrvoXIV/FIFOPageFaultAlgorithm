#include <stdio.h>
#include <stdlib.h>

#define INVALID_ARG_LENGTH 1
#define INVALID_REF_FILE 2
#define INVALID_MEM_SIZE 3

void print_errors(int error_code) {
    switch(error_code) {
        case INVALID_ARG_LENGTH:
            /* handle here */
            break;
        case INVALID_REF_FILE:
            /* handle here */
            break;
        case INVALID_MEM_SIZE:
            /* handle here */
            break;
    }
}

int is_valid_commands(int argc, char* argv[]) {
    if(argc != 3) {
        print_errors(INVALID_ARG_LENGTH);
        return INVALID_ARG_LENGTH;
    }
    char* ref_file_name = argv[1];
    FILE* ref_file = fopen(ref_file_name, "r");
    if(ref_file == NULL) {
        print_errors(INVALID_REF_FILE);
        return INVALID_REF_FILE;
    }
    char* mem_size_arg = argv[2];
    int mem_size = atoi(mem_size_arg);
    if(mem_size < 1 || mem_size > 10) {
        print_errors(INVALID_MEM_SIZE);
        return INVALID_MEM_SIZE;
    }
}

int main(int argc, char* argv[]) {
    is_valid_commands(argc, argv); // command validator
}
