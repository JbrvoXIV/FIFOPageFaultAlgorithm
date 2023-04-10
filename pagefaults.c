
/*
    This program takes 3 arguments - the name of the program executable, the name of the text file containing the ref
    string, and the size of memory. It will then read the contents of the file and perform the FIFO algorithm on it
    to determine the number of page faults and the final state of memory frames.

    The program firstly checks for any run time errors regarding the command line arguments and the contents of the file
    (is_valid_commands and print_errors). Once verified, the program will attempt to read the contents of the file, counting
    the amount of integers inside said file to initialize the array that will store the ref string. If no errors in
    reading the file are present, it will then read the contents of the file into an array and initialize the array that
    will contain the page frames, occupying it before ultimately running the FIFO algorithm. When the FIFO algorithm is called,
    the program will traverse the page_ref array and compare the contents with the page_frames array. The amount of page
    faults will be tracked through a local variable. The 0th index of the page_frames array is the oldest, while the
    mem_size - 1 index is the youngest. If the page reference is not present in the page_frames array, it will call a
    helper method that will restructure the array and add the page reference as the youngest and remove the head. This
    will continue all the way until we reach the end of the page_ref array. Once the end of the array has been reached,
    a helper method to print the results and the contents of the page_ref array will be called.

    NAME: Jordan Bravo
    PantherID: 6337381
 */

#include <stdio.h>
#include <stdlib.h>

#define INVALID_ARG_LENGTH 1
#define INVALID_REF_FILE 2
#define INVALID_MEM_SIZE 3

int file_size; // set on successful check and read of file containing reference string

void print_errors(int error_code) { // print errors reported by is_valid_commands
    switch(error_code) {
        case INVALID_ARG_LENGTH:
            fprintf(stderr, "ERROR: Invalid argument's -> EXPECTED: ./program_name pageref_file mem_size.\n");
            break;
        case INVALID_REF_FILE:
            fprintf(stderr, "ERROR: Invalid file, please input a valid file for page ref's.\n");
            break;
        case INVALID_MEM_SIZE:
            fprintf(stderr, "ERROR: Invalid memory size, please input an integer between 1 and 10.\n");
            break;
    }
}

int is_valid_commands(int argc, char* argv[]) { // parse command line arguments and look for errors
    if(argc != 3) { // not enough legal arguments
        print_errors(INVALID_ARG_LENGTH);
        return INVALID_ARG_LENGTH;
    }
    char* ref_file_name = argv[1];
    FILE* ref_file = fopen(ref_file_name, "r");
    if(ref_file == NULL) { // file doesn't exist
        print_errors(INVALID_REF_FILE);
        return INVALID_REF_FILE;
    }
    char* mem_size_arg = argv[2];
    int mem_size = atoi(mem_size_arg);
    if(mem_size < 1 || mem_size > 10) { // mem_size is either wrong input or wrong amount
        print_errors(INVALID_MEM_SIZE);
        return INVALID_MEM_SIZE;
    }
    return 0; // no err
}

int validate_file_contents(FILE* pageref_file) { // check if file contains ints separated by white space
    int num, size = 0;
    while(fscanf(pageref_file, "%d", &num) != EOF) {
        size++;
    }

    if(ferror(pageref_file)) {
        fprintf(stderr, "There was an error reading from the file, please check the contents.\n");
        return -1;
    }

    file_size = size;
    return size;
}

void initialize_page_frames(int page_frames[], int mem_size) { // set page frames array occupated with 0 -> mem_size - 1 frames
    for(int i = 0; i < mem_size; i++)
        page_frames[i] = i;
}

void read_contents_into_arr(FILE* pageref_file, int page_ref[file_size]) { // after file is found, read from file to page_ref
    int num, i = 0;
    while(fscanf(pageref_file, "%d", &num) != EOF)
        page_ref[i++] = num;
}

// check if current page ref is resident in page_frames array
int check_for_residence(int ref_page, const int page_frames[], int mem_size) {
    for(int i = 0; i < mem_size; i++)
        if(page_frames[i] == ref_page)
            return 1;
    return 0;
}

// shuffle page frames array so that head is removed and new tail is ref_page
void shuffle_page_frames(int ref_page, int page_frames[], int mem_size) {
    for(int i = 0; i < mem_size - 1; i++)
        page_frames[i] = page_frames[i + 1];
    page_frames[mem_size - 1] = ref_page;
}

// print results from fifo algorithm
void print_results(int page_faults, const int page_frames[], int mem_size) {
    printf("\nFIFO: %d page faults\n", page_faults);
    printf("Final state of memory:");
    for(int i = 0; i < mem_size; i++)
        printf("%2d", page_frames[i]);
}

// function that runs FIFO algorithm on a reference string with mem_size page_frames array
void fifo_algorithm(int page_frames[], const int page_ref[file_size], int mem_size) {
    int page_faults = 0;
    for(int i = 0; i < file_size; i++) { // go through each reference page in ref str
        int ref_page = page_ref[i]; // current ref page
        int is_resident = check_for_residence(ref_page, page_frames, mem_size);
        if(is_resident == 0) { // ref not in page_frames array, shuffle and increase page_faults counter
            shuffle_page_frames(ref_page, page_frames, mem_size);
            page_faults++;
        }
    }
    print_results(page_faults, page_frames, mem_size);
}

// function to print page ref
void print_page_ref(const int page_ref[file_size]) {
    printf("Reference string:");
    for(int i = 0; i < file_size; i++)
        printf("%2d", page_ref[i]);
}

int main(int argc, char* argv[]) {
    int err_code = is_valid_commands(argc, argv); // command validator
    if(err_code != 0) // error was found
        return err_code;

    FILE* pageref_file = fopen(argv[1], "r");
    int mem_size = atoi(argv[2]);
    int is_valid_contents = validate_file_contents(pageref_file); // validator for file
    if(is_valid_contents < 0) { // file validator returned error
        fclose(pageref_file);
        return is_valid_contents;
    }

    if(fseek(pageref_file, 0, SEEK_SET)) { // set file pointer to start, return -1 if error
        fprintf(stderr, "Error setting file pointer back to start.\n");
        return -1;
    }

    int page_ref[file_size]; // array for reference string of file_size size
    int page_frames[mem_size]; // array for page frames of size mem_size

    initialize_page_frames(page_frames, mem_size);
    read_contents_into_arr(pageref_file, page_ref);

    print_page_ref(page_ref);

    fifo_algorithm(page_frames, page_ref, mem_size);

    fclose(pageref_file);
}
