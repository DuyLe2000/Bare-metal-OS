#include "printf.h"
#include "string.h"
#include "handler.h"

#define MAX_STR_LEN 256

char user_input[MAX_STR_LEN];
char child_strings[10][10];

/**
 * Main function ----------------------------------------------------------------
 */
void main(){
	uart_puts("\033[32m####### ####### ####### #######\033[33m  #####  #        #####    ###\n");
	uart_puts("\033[32m#       #       #          #   \033[33m #     # #    #  #     #  #   #\n");
	uart_puts("\033[32m#       #       #          #   \033[33m       # #    #  #     # #     #\n");
	uart_puts("\033[32m#####   #####   #####      #   \033[33m  #####  #    #   ###### #     #\n");
	uart_puts("\033[32m#       #       #          #   \033[33m #       #######       # #     #\n");
	uart_puts("\033[32m#       #       #          #   \033[33m #            #  #     #  #   #\n");
	uart_puts("\033[32m####### ####### #######    #   \033[33m #######      #   #####    ###\n\n");

	uart_puts("\033[32m######     #    ######  #######\033[35m   #####    #####\n");
	uart_puts("\033[32m#     #   # #   #     # #      \033[35m  #     #  #     #\n");
	uart_puts("\033[32m#     #  #   #  #     # #      \033[35m  #     #  #\n");
	uart_puts("\033[32m######  #     # ######  #####  \033[35m  #     #   #####\n");
	uart_puts("\033[32m#     # ####### #   #   #      \033[35m  #     #        #\n");
	uart_puts("\033[32m#     # #     # #    #  #      \033[35m  #     #  #     #\n");
	uart_puts("\033[32m######  #     # #     # #######\033[35m   #####    #####\n\n");

	uart_puts("\033[36mDeveloped by Le Ngoc Duy - s3757327\n");
	uart_puts("\n");
	reset_color();

	// Test the command printf 
	printf("Using printf to print a decimal value: %d \n", 100);
	printf("Using printf to print a character: %c \n", 'A');
	printf("Using printf to print a string: ");
	printf("%s", "Hello world!\n");
	printf("Using printf to print a hex numebr: ");
	printf("%x", 0x123ABC);
	printf("\n\n");

	// set up serial console
	printf("System initialization: \n");
	uart_init();
	framebf_init();

	// Testing site for command out of the while loop ------------------------------------------------------



	// End of Testing site for command out of the while loop ---------------------------------------------

	// Main loop of program
	while (1){
		uart_puts("\nBare OS> ");

		//Loop to get words until enter key is pressed
		while (1){
			//read each char
			char c = uart_getc();

			if (c == '\n'){
				// printf("\nUser input is: ");
				// printf(user_input);
				printf("\n");

				split_string(user_input, child_strings);
				handle_input(child_strings);

				reset_string(user_input);
				for (int i = 0; i < 10; i++){
					reset_string(child_strings[i]);
				}
				break;
			} 
			// This is to handle the backspace key to delete a character
			// 8 is is decimal value of ASCII for backspace character 
			else if (c == 8){
				int i = 0;
				while (1){
					if (user_input[i] == '\0'){
						user_input[i - 1] = '\0';
						break;
					}
					i++;
				}

				printf("\033[2K\r"); // print this to clear input and return cursor to the beginning of line 
				printf("Bare OS> ");
				printf(user_input);
			}
			else {
				//send back input char and string
				append_char(user_input, c);
				printf("\033[2K\r"); // print this to clear input and return cursor to the beginning of line 
				printf("Bare OS> ");
				printf(user_input);
			}
		}
	}
}

// Testing testing hello hello 