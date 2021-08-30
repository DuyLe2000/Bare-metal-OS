#include "printf.h"
#include "string.h"

/**
 * A function to calculate the length of a string ------------------------------------
 */
int len(char *str){
	int count = 0;

	for (str; *str != '\0'; str++){
		count++;
	}

	return count;
}

/**
 * A function to add a char to the end of a string ----------------------------------
 */
void append_char(char *str, char c){
    while(1){
        if (*str == '\0'){
            *str = c;
            break;
        } else {
            str++;
        }
    }
}

/**
 * A function to copy one array to another ------------------------------------------- 
 */
void copy_string(char *original_str, char *copy_str){
    while (*original_str != '\0'){
        append_char(copy_str, *original_str);
        original_str++;
        copy_str++;
    }
}

/**
 * A function to clear a string -------------------------------------------------
 */
void reset_string(char *str){
    for (str; *str != '\0'; str++){
        *str = '\0';
    }
}

/**
 * A function to compare two strings -----------------------------------------------
 */
int compare_string(char* str1, char* str2){
    int flag = 1;

    // Loop through both strings
    while (*str1 != '\0' || *str2 != '\0') {
        if (*str1 == *str2) {
            str1++;
            str2++;
        }

        // Check if characters are not the same
        else if ((*str1 == '\0' && *str2 != '\0')
                 || (*str1 != '\0' && *str2 == '\0')
                 || *str1 != *str2) {
            flag = 0;
            // printf("Unequal string\n");
            break;
        }
    }

    if (flag == 1){
        // printf("Equal string\n");
    }

    return flag;
}

/**
 * A function to make a substring from a parent string -----------------------------------------------
 */
void make_substring(char *parent_str, char *child_str, int starting_pos, char ending_char){
    while (1){
        *child_str = *(parent_str + starting_pos);

        child_str++;
        parent_str++;

        if (*parent_str == ending_char){
            break;
        }
    }
}

/**
 * A function to split a string into many strings ------------------------------------------
 */
void split_string(char input[], char child_strings[10][10]){
    int char_num = 0;
    int word_num = 0;
	int input_len = len(input);

	for (int i = 0; i <= input_len; i++){
        // if space or NULL found, assign NULL into child_strings[word_num]
        if (input[i] == ' ' || input[i] == '\0'){
            child_strings[word_num][char_num] = '\0';
            word_num++; //for next word
            char_num = 0; //for next word, init index to 0
        } else {
            child_strings[word_num][char_num] = input[i];
            char_num++;
        }
    }
}

/**
 * A function to convert from string to integer ------------------------------------------
 */
int atoi(char *str){
    int x = 0;

    for (str; *str != '\0'; str++){
        if (*str < '0' || *str > '9'){
            return -1;
        }

        x = x*10 + *str - '0';
    }
    return x;
}