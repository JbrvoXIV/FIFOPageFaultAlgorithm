#include <stdio.h>
#include <stdlib.h>

#define INVALID_ARG_LENGTH 1
#define INVALID_REF_FILE 2
#define INVALID_MEM_SIZE 3

int file_size;

void print_errors(int error_code) {
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
    return 0; // no err
}

int validate_file_contents(FILE* pageref_file) {
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

void initialize_page_frames(int page_frames[], int mem_size) {
    for(int i = 0; i < mem_size; i++)
        page_frames[i] = i;
}

void read_contents_into_arr(FILE* pageref_file, int page_ref[file_size]) {
    int num, i = 0;
    while(fscanf(pageref_file, "%d", &num) != EOF)
        page_ref[i++] = num;
}

int check_for_residence(int ref_page, const int page_frames[], int mem_size) {
    for(int i = 0; i < mem_size; i++)
        if(page_frames[i] == ref_page)
            return 1;
    return 0;
}

void shuffle_page_frames(int ref_page, int page_frames[], int mem_size) {
    for(int i = 0; i < mem_size - 1; i++)
        page_frames[i] = page_frames[i + 1];
    page_frames[mem_size - 1] = ref_page;
}

void print_results(int page_faults, const int page_frames[], int mem_size) {
    printf("\nFIFO: %d page faults\n", page_faults);
    printf("Final state of memory:");
    for(int i = 0; i < mem_size; i++)
        printf("%2d", page_frames[i]);
}

void fifo_algorithm(int page_frames[], int page_ref[file_size], int mem_size) {
    int page_faults = 0;
    for(int i = 0; i < file_size; i++) { // go through each reference page in ref str
        int ref_page = page_ref[i]; // current ref page
        int is_resident = check_for_residence(ref_page, page_frames, mem_size);
        if(is_resident == 0) {
            shuffle_page_frames(ref_page, page_frames, mem_size);
            page_faults++;
        }
    }
    print_results(page_faults, page_frames, mem_size);
}

void print_page_ref(int page_ref[file_size]) {
    for(int i = 0; i < file_size; i++)
        printf("%2d", page_ref[i]);
}

int main(int argc, char* argv[]) {
    int err_code = is_valid_commands(argc, argv); // command validator
    if(err_code != 0)
        return err_code;

    FILE* pageref_file = fopen(argv[1], "r");
    int mem_size = atoi(argv[2]);
    int is_valid_contents = validate_file_contents(pageref_file);
    if(is_valid_contents < 0) {
        fclose(pageref_file);
        return is_valid_contents;
    }

    if(fseek(pageref_file, 0, SEEK_SET)) {
        fprintf(stderr, "Error setting file pointer back to start.\n");
        return -1;
    }

    int page_ref[file_size];
    int page_frames[mem_size];

    initialize_page_frames(page_frames, mem_size);
    read_contents_into_arr(pageref_file, page_ref);

    print_page_ref(page_ref);

    fifo_algorithm(page_frames, page_ref, mem_size);

    fclose(pageref_file);
}
