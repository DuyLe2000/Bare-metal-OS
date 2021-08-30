#include "printf.h"
#include "uart.h"

#define MAX_PRINT_SIZE 256


void printf(char *string,...) {

	va_list ap;
	va_start(ap, string);

	char buffer[MAX_PRINT_SIZE];
	int buffer_index = 0;

	char temp_buffer[MAX_PRINT_SIZE];

	//Rest buffer value
	for(int i = 0; i < MAX_PRINT_SIZE; i++) {
		buffer[i] = '\0';
	}

	//Rest temp buffer value
	for(int i = 0; i < MAX_PRINT_SIZE; i++) {
		temp_buffer[i] = '\0';
	}

	while(1) {
		if (*string == 0)
			break;
		
		if (*string == '%') {
			string++;

			// DECIMAL FORMAT ----------------------------------------------------
			if (*string == 'd') {
				string++;
				int x = va_arg(ap, int);
				int temp_index = MAX_PRINT_SIZE - 1;

				do {
					temp_buffer[temp_index] = (x % 10) + '0';
					temp_index--;
					x /= 10;
				} while(x != 0);

				for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
					buffer[buffer_index] = temp_buffer[i];
					buffer_index++;
				}
			}
			// HEX FORMAT -------------------------------------------------------------
			else if (*string == 'x') {
				string++;
				int x = va_arg(ap, int);
				int n = 0;

				// Set the fist two element of buffer to "0x"
				buffer[0] = '0';
				buffer[1] = 'x';

				// Loop through the input hex number and put it in the buffer
				for (int c = 28; c >= 0; c = c - 4) {
					// Get highest 4-bit nibble
					n = (x >> c) & 0xF;

					// Keep numbers from 0 to 9 as it while change numbers 10 to 16 as A -> F
					n += (n > 9) ? (- 10 + 'A') : '0';
					buffer[buffer_index + 2] = n;
					buffer_index++;
				}
			}
			// CHAR FORMAT ------------------------------------------------------
			else if (*string == 'c') {
				string++;
				int x = va_arg(ap, int);

				buffer[buffer_index] = x;
				buffer_index++;
			}
			// STRING FORMAT ---------------------------------------------------------------------
			if (*string == 's') {
				string++;
				char *arg_string = va_arg(ap, char*);

				// Copy temp buffer to buffer
				for (int i = 0; *(arg_string + i) != '\0'; i++) {
					buffer[i] = *(arg_string + i);
				}
			}
		}
		else {
			buffer[buffer_index] = *string;
			buffer_index++;
			string++;
		}

		if (buffer_index == MAX_PRINT_SIZE - 1)
			break;
	}

	va_end(ap);


	//Print out formated string
	uart_puts(buffer);
}
