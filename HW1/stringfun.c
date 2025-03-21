#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int count_words(char *, int);
//add additional prototypes here

void reverse_string(char *, int);
void word_print(char *, int);

int setup_buff(char *buff, char *user_str, int len) {
    //TODO: #4: Implement the setup buff as per the directions
    int str_length = strlen(user_str);

    //check if the input string exceeds buffer size
    if (len > BUFFER_SZ) len = BUFFER_SZ;

    int bufferPos = 0;
    int consecutiveSpace = 0;

    //skip leading spaces and quotation marks
    int startPos = 0;
    while (startPos < str_length && (user_str[startPos] == ' ' || user_str[startPos] == '\t' || user_str[startPos] == '"')) {
        startPos++;
    }

    //makes sure consecutive spaces are replaced with only one space
    for (int i = startPos; i < str_length; ++i) {
        char currentChar = user_str[i];


        if (currentChar == ' ' || currentChar == '\t') {
            if (!consecutiveSpace) {
                bufferPos++;
                consecutiveSpace = 1;
            }
        } else {
            bufferPos++;
            consecutiveSpace = 0;
        }

        if (bufferPos >= BUFFER_SZ - 1) {
            return -1;  // Processed string exceeds buffer size
        }
    }

    bufferPos = 0;
    consecutiveSpace = 0;

    //process the string into the buffer
    for (int i = startPos; i < str_length; ++i) {
        char currentChar = user_str[i];

        if (currentChar == '"') {
            continue;
        }

        if (currentChar == ' ' || currentChar == '\t') {
            if (!consecutiveSpace) {
                buff[bufferPos++] = ' ';
                consecutiveSpace = 1;
            }
        } else {
            buff[bufferPos++] = currentChar;
            consecutiveSpace = 0;
        }
    }

    //remove space at the end
    if (bufferPos > 0 && buff[bufferPos - 1] == ' ') {
        --bufferPos;
    }

    buff[bufferPos] = '\0';

    return bufferPos;
}


//reverse string function
void reverse_string(char *buff, int len) {
    int start = 0;
    while (start < len && buff[start] == ' ') {
        start++; // skip leading spaces
    }
    for (int i = start, j = len - 1; i < j; i++, j--) {
        char temp = buff[i];
        buff[i] = buff[j];
        buff[j] = temp;
    }
}

// print each word
void word_print(char *buff, int len) {
    printf("Word Print\n----------\n");

    int word_count = 0;
    int word_start = 0;
    int in_word = 0;

    for (int i = 0; i <= len; i++) {
        char current_char = buff[i];

        //detect end of the word
        if ((current_char == ' ' || current_char == '\0') && in_word) {
            in_word = 0;
            word_count++;
            printf("%d. %.*s(%d)\n", word_count, i - word_start, buff + word_start, i - word_start);
        }

        // detect start of a word
        if (current_char != ' ' && current_char != '\0' && !in_word) {
            in_word = 1;
            word_start = i;
        }
    }

    printf("\nNumber of words returned: %d\n", word_count);
}

//print buffer and bracket
void print_buff(char *buff, int len) {
    printf("Buffer: [");
    for (int i = 0; i < len; i++) {
        putchar(buff[i]);
    }
    for (int i = len; i < BUFFER_SZ; i++) {
        putchar('.');
    }
    printf("]\n");
}

void usage(char *exename) {
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

// count words in buffer
int count_words(char *buff, int len) {
    //YOU MUST IMPLEMENT
    int word_count = 0;
    int in_word = 0;

    //iterate buffer and count words
    for (int i = 0; i < len; i++) {
        char current_char = buff[i];

        //start of a word
        if ((current_char != ' ' && current_char != '\t') && !in_word) {
            word_count++;
            in_word = 1;  
        }
        
        else if (current_char == ' ' || current_char == '\t') {
            in_word = 0;
        }
    }

    return word_count;
}

int main(int argc, char *argv[]) {
    char *buff;             // placeholder for the internal buffer
    char *input_string;     // holds the string provided by the user on cmd line
    char opt;               // used to capture user option from cmd line
    int rc;                 // used for return codes
    int user_str_len;       // length of user-supplied string

    //TODO: #1, WHY IS THIS SAFE, aka what if arv[1] does not exist?
    // PLACE A COMMENT BLOCK HERE EXPLAINING
    //It checks to make sure the program has enough aruments before using argv[1]. If argc < 2, 
    //it means that argv[1] does not exist, so the program will exit. I believe it prevents the program
    //from crashing because it tries to access a missing argument.

    if ((argc < 2) || (*argv[1] != '-')) {
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1] + 1);  //get the option flag

    //handle the help flag and exit normally
    if (opt == 'h') {
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO: #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // This makes sure the program has enough arguments to proceed. 
    //The program makes sure it has the name, a flag, and a string.
    //If there is less than 3 arguments, it means the user didn't provide one of the following.
    //The program will exit to avoid any errors.
    if (argc < 3) {
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2];  //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3

    buff = (char *)malloc(BUFFER_SZ);  
    if (buff == NULL) {
        printf("Error allocating memory for buffer.\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ); //see todos
    if (user_str_len < 0) {
        free(buff);
        exit(0); 
    }

    switch (opt) {
        case 'c':
            rc = count_words(buff, user_str_len);  //you need to implement
            if (rc < 0) {
                printf("Error counting words, rc = %d", rc);
                free(buff);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO: #5 Implement the other cases for 'r' and 'w' by extending
        //      the case statement options

        case 'r':
            reverse_string(buff, user_str_len);  //reverse string
            break;

        case 'w':
            word_print(buff, user_str_len);  //print words
            break;

        case 'x':
            printf("Not Implemented!\n");
            free(buff);
            exit(0);

        default:
            usage(argv[0]);
            free(buff);
            exit(1);
    }

    
    print_buff(buff, user_str_len);
    //TODO: #6 Dont forget to free your buffer before exiting
    free(buff);
    exit(0);

    
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
// I think providing both the pointer and the length is good practice because
//the functions will be reusable if the buffer size changes. It also avoids 
//the risk of corruption with boundaries on how much of the buffer is processed.