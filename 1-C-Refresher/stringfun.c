#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    int buff_index = 0; // Tracks the position in the buffer
    char prev_char = '\0'; // Tracks the last added character to handle whitespace
    char *ptr = user_str; // Pointer to traverse the user string

    // Iterate through the user string
    while (*ptr != '\0') {
        if (buff_index >= len) {
            return -1; // User string is too large
        }

        // Handle non-whitespace characters
        if (*ptr != ' ' && *ptr != '\t') {
            buff[buff_index++] = *ptr;
            prev_char = *ptr;
        }
        // Handle whitespace: Add a single space if the last character was not a space
        else if (prev_char != ' ') {
            buff[buff_index++] = ' ';
            prev_char = ' ';
        }
        ptr++;
    }

    // Fill the remaining buffer with '.' characters
    for (; buff_index < len; buff_index++) {
        buff[buff_index] = '.';
    }

    return (ptr - user_str);
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int str_len){
    int word_count = 0;
    int in_word = 0; //Boolean for if currently in/out of a word

    for (int i = 0; i < str_len; i++) { //Iterate thru string
        if (buff[i] != ' ') {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0; 
        }
    }

    return word_count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    /* 
        The condition (argc < 2) ensures that argv[1] is valid and by combining the two conditions, the code ensures saftey
        by preventing invalid memeory access. 
    */
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      Ensures the user provides both a valid option flag and a string.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        exit(99);
    }
    
    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            printf("Reversed String: ");
            for (int i = user_str_len - 1; i >= 0; i--) {
                putchar(buff[i]); 
            }
            putchar('\n');
            break;

        case 'w': {
            int word_index = 1;
            int char_count = 0;
            int in_word = 0;

            printf("Word Print\n----------\n");
            for (int i = 0; i < user_str_len; i++) {
                if (buff[i] != ' ') {
                    if (!in_word) {
                        // Start of a new word
                        printf("%d. ", word_index++);
                        in_word = 1;
                    }
                    putchar(buff[i]);
                    char_count++;
                } else if (in_word) {
                    // End of a word
                    printf(" (%d)\n", char_count);
                    char_count = 0;
                    in_word = 0;
                }
            }

            // Handle the last word if it ends without a space
            if (in_word) {
                printf(" (%d)\n", char_count);
            }
            break;
        }
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          If the buffer changes sizes in the future, the program won't rely on a hard-coded value
//          and it improves readability. 