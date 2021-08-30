#include "function_handler.h"
#include "mbox.h"
#include "uart.h"

//Use RGBA32 (32 bits for each pixel)
#define COLOR_DEPTH 32

//Pixel Order: BGR in memory order (little endian --> RGB in byte order)
#define PIXEL_ORDER 0

//Screen info
int actual_width, actual_height, pitch;

/* Frame buffer address
 * (declare as pointer of unsigned char to access each byte) */
unsigned char *fb;

void clear_screen(){
    printf("\033[2J\033[1;1H");
}

void reset_color(){
	printf("\033[0m");
}

void set_color(char *option, char *color){
	int is_executed = 0;

	// Color choices array
	char color_list[][10] = { "BLACK", "RED", "GREEN", "YELLOW", "BLUE", "MAGNETA", "CYAN", "WHITE" };

	// For set text color
	if (compare_string(option, "-t")){
		for (int i = 0; i < 8; i++){
			if (compare_string(color, color_list[i])){
				printf("\033[3%cm", (48 + i));
				printf("\n");
				printf("Text color set to: ");
				printf(color_list[i]);
				printf("\n");
				is_executed = 1;
				break;
			}
		}
		if (!is_executed){
			printf("ERROR: Wrong color choice.\n");
			printf("Supported colors are: BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE only.\n");
			is_executed = 1;
		}
	} 

	// For set background color
	if (compare_string(option, "-b")){
		for (int i = 0; i < 8; i++){
			if (compare_string(color, color_list[i])){
				printf("\033[4%cm", (48 + i));
				printf("\n");
				printf("Background color set to: ");
				printf(color_list[i]);
				printf("\n");
				is_executed = 1;
				break;
			}
		}
		if (!is_executed){
			printf("ERROR: Wrong color choice.\n");
			printf("Supported colors are: BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE only.\n");
			is_executed = 1;
		}
	}
}

void get_board_revision(){
    mbox[0] = 7*4;          // Message Buffer Size in bytes (9 elements * 4 bytes (32 bit) each)
	mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
	mbox[2] = 0x00010002;   // TAG Identifier: Get clock rate
	mbox[3] = 4;            // Value buffer size in bytes (max of request and response lengths)
	mbox[4] = 0;			// REQUEST CODE = 0
	mbox[5] = 0;            // clear output buffer
	mbox[6] = MBOX_TAG_LAST;

	if (mbox_call(ADDR(mbox), MBOX_CH_PROP)) {
		uart_puts("Response Code for whole message: ");
		uart_hex(mbox[1]);
		uart_puts("\n");

		uart_puts("Response Code in Message TAG: ");
		uart_hex(mbox[4]);
		uart_puts("\n");

		uart_puts("DATA: board revision = ");
		uart_hex(mbox[5]);
		uart_puts("\n");

	} else {
		uart_puts("Unable to query!\n");
	}
}

void get_board_model(){
    mbox[0] = 7*4;          // Message Buffer Size in bytes (4 elements * 4 bytes (32 bit) each)
	mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
	mbox[2] = 0x00010001;   // TAG Identifier: Get board model
	mbox[3] = 4;            // Value buffer size in bytes (max of request and response lengths)
	mbox[4] = 0;			// REQUEST CODE = 0
	mbox[5] = 0;            // clear output buffer
	mbox[6] = MBOX_TAG_LAST;

	if (mbox_call(ADDR(mbox), MBOX_CH_PROP)) {
		uart_puts("Response Code for whole message: ");
		uart_hex(mbox[1]);
		uart_puts("\n");

		uart_puts("Response Code in Message TAG: ");
		uart_hex(mbox[4]);
		uart_puts("\n");

		uart_puts("DATA: board model = ");
		uart_hex(mbox[5]);
		uart_puts("\n");

	} else {
		uart_puts("Unable to query!\n");
	}
}

void get_mac_address(){
	mbox[0] = 7*4;          // Message Buffer Size in bytes (4 elements * 4 bytes (32 bit) each)
	mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
	mbox[2] = 0x00010003;   // TAG Identifier: Get board model
	mbox[3] = 6;            // Value buffer size in bytes (max of request and response lengths)
	mbox[4] = 0;			// REQUEST CODE = 0
	mbox[5] = 0;            // clear output buffer
	mbox[6] = MBOX_TAG_LAST;

	if (mbox_call(ADDR(mbox), MBOX_CH_PROP)) {
		uart_puts("Response Code for whole message: ");
		uart_hex(mbox[1]);
		uart_puts("\n");

		uart_puts("Response Code in Message TAG: ");
		uart_hex(mbox[4]);
		uart_puts("\n");

		uart_puts("DATA: MAC address = ");
		uart_hex(mbox[5]);
		uart_puts("\n");

	} else {
		uart_puts("Unable to query!\n");
	}
}

void get_arm_memory(){
	mbox[0] = 8*4;          // Message Buffer Size in bytes (4 elements * 4 bytes (32 bit) each)
	mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
	mbox[2] = 0x00010005;   // TAG Identifier: Get board model
	mbox[3] = 8;            // Value buffer size in bytes (max of request and response lengths)
	mbox[4] = 0;			// REQUEST CODE = 0
	mbox[5] = 0;            // clear output buffer
	mbox[6] = 0;
	mbox[7] = MBOX_TAG_LAST;

	if (mbox_call(ADDR(mbox), MBOX_CH_PROP)) {
		uart_puts("Response Code for whole message: ");
		uart_hex(mbox[1]);
		uart_puts("\n");

		uart_puts("Response Code in Message TAG: ");
		uart_hex(mbox[4]);
		uart_puts("\n");

		uart_puts("DATA: ARM memory = ");
		uart_hex(mbox[6]);
		uart_puts("\n");
	} else {
		uart_puts("Unable to query!\n");
	}
}

void get_virtual_screen_size(){
	mbox[0] = 8*4;          // Message Buffer Size in bytes (4 elements * 4 bytes (32 bit) each)
	mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
	mbox[2] = 0x00040004;   // TAG Identifier: Get board model
	mbox[3] = 8;            // Value buffer size in bytes (max of request and response lengths)
	mbox[4] = 0;			// REQUEST CODE = 0
	mbox[5] = 0;            // clear output buffer
	mbox[6] = 0;
	mbox[7] = MBOX_TAG_LAST;

	if (mbox_call(ADDR(mbox), MBOX_CH_PROP)) {
		uart_puts("Response Code for whole message: ");
		uart_hex(mbox[1]);
		uart_puts("\n");

		uart_puts("Response Code in Message TAG: ");
		uart_hex(mbox[4]);
		uart_puts("\n");

		uart_puts("DATA: virtual width = ");
		uart_hex(mbox[5]);
		uart_puts("\n");
		uart_puts("DATA: virtual height = ");
		uart_hex(mbox[6]);
		uart_puts("\n");
	} else {
		uart_puts("Unable to query!\n");
	}
}

void framebf_init() {
	mbox[0] = 35 * 4; // Length of message in bytes
	mbox[1] = MBOX_REQUEST;
	mbox[2] = MBOX_TAG_SETPHYWH; //Set physical width-height
	mbox[3] = 8; // Value size in bytes
	mbox[4] = 0; // REQUEST CODE = 0
	mbox[5] = 1024; // Value(width)
	mbox[6] = 600; // Value(height)
	mbox[7] = MBOX_TAG_SETVIRTWH; //Set virtual width-height
	mbox[8] = 8;
	mbox[9] = 0;
	mbox[10] = 1024;
	mbox[11] = 600;
	mbox[12] = MBOX_TAG_SETVIRTOFF; //Set virtual offset
	mbox[13] = 8;
	mbox[14] = 0;
	mbox[15] = 0; // x offset
	mbox[16] = 0; // y offset
	mbox[17] = MBOX_TAG_SETDEPTH; //Set color depth
	mbox[18] = 4;
	mbox[19] = 0;
	mbox[20] = COLOR_DEPTH; //Bits per pixel
	mbox[21] = MBOX_TAG_SETPXLORDR; //Set pixel order
	mbox[22] = 4;
	mbox[23] = 0;
	mbox[24] = PIXEL_ORDER;
	mbox[25] = MBOX_TAG_GETFB; //Get frame buffer
	mbox[26] = 8;
	mbox[27] = 0;
	mbox[28] = 16; //alignment in 16 bytes
	mbox[29] = 0; //will return Frame Buffer size in bytes
	mbox[30] = MBOX_TAG_GETPITCH; //Get pitch
	mbox[31] = 4;
	mbox[32] = 0;
	mbox[33] = 0; //Will get pitch value here
	mbox[34] = MBOX_TAG_LAST;

	// Call Mailbox
	if (mbox_call(ADDR(mbox), MBOX_CH_PROP) //mailbox call is successful ?
		&& mbox[20] == COLOR_DEPTH //got correct color depth ?
		&& mbox[24] == PIXEL_ORDER //got correct pixel order ?
		&& mbox[28] != 0 //got a valid address for frame buffer ?
		) {
		/* Convert GPU address to ARM address (clear higher address bits)
		 * Frame Buffer is located in RAM memory, which VideoCore MMU
		 * maps it to bus address space starting at 0xC0000000.
		 * Software accessing RAM directly use physical addresses
		 * (based at 0x00000000)
		 */
		mbox[28] &= 0x3FFFFFFF;

		// Access frame buffer as 1 byte per each address
		fb = (unsigned char*) ((unsigned long) mbox[28]);
		uart_puts("Got allocated Frame Buffer at RAM physical address: ");
		uart_hex(mbox[28]);
		uart_puts("\n");
		uart_puts("Frame Buffer Size (bytes): ");
		uart_dec(mbox[29]);
		uart_puts("\n");
		actual_width = mbox[5]; // Actual physical width
		actual_height = mbox[6]; // Actual physical height
		pitch = mbox[33]; // Number of bytes per line
	} else {
		uart_puts("Unable to get a frame buffer with provided setting\n");
	}
}

int check_int(char *str){
	int is_int = 1;
	for (str; *str != '\0'; str++){
		if (*str >= '0' && *str <= '9'){
			is_int = 1;
		} else {
			is_int = 0;
			break;
		}
	}
	return is_int;
}

void set_screen_size_with_option(char *str, int width, int height){
	mbox[0] = 35 * 4; // Length of message in bytes
	mbox[1] = MBOX_REQUEST;
	mbox[2] = MBOX_TAG_SETPHYWH; //Set physical width-height
	mbox[3] = 8; // Value size in bytes
	mbox[4] = 0; // REQUEST CODE = 0
	if (compare_string(str, "-p")){
		uart_puts("Set custom physical size\n");
		mbox[5] = width;
		mbox[6] = height;
	}
	mbox[7] = MBOX_TAG_SETVIRTWH; //Set virtual width-height
	mbox[8] = 8;
	mbox[9] = 0;
	if (compare_string(str, "-v")){
		uart_puts("Set custom virtual size\n");
		mbox[10] = width;
		mbox[11] = height;
	}
	mbox[12] = MBOX_TAG_SETVIRTOFF; //Set virtual offset
	mbox[13] = 8;
	mbox[14] = 0;
	mbox[15] = 0; // x offset
	mbox[16] = 0; // y offset
	mbox[17] = MBOX_TAG_SETDEPTH; //Set color depth
	mbox[18] = 4;
	mbox[19] = 0;
	mbox[20] = COLOR_DEPTH; //Bits per pixel
	mbox[21] = MBOX_TAG_SETPXLORDR; //Set pixel order
	mbox[22] = 4;
	mbox[23] = 0;
	mbox[24] = PIXEL_ORDER;
	mbox[25] = MBOX_TAG_GETFB; //Get frame buffer
	mbox[26] = 8;
	mbox[27] = 0;
	mbox[28] = 16; //alignment in 16 bytes
	mbox[29] = 0; //will return Frame Buffer size in bytes
	mbox[30] = MBOX_TAG_GETPITCH; //Get pitch
	mbox[31] = 4;
	mbox[32] = 0;
	mbox[33] = 0; //Will get pitch value here
	mbox[34] = MBOX_TAG_LAST;

	// Call Mailbox
	if (mbox_call(ADDR(mbox), MBOX_CH_PROP) //mailbox call is successful ?
		&& mbox[20] == COLOR_DEPTH //got correct color depth ?
		&& mbox[24] == PIXEL_ORDER //got correct pixel order ?
		&& mbox[28] != 0 //got a valid address for frame buffer ?
		) {
		/* Convert GPU address to ARM address (clear higher address bits)
		 * Frame Buffer is located in RAM memory, which VideoCore MMU
		 * maps it to bus address space starting at 0xC0000000.
		 * Software accessing RAM directly use physical addresses
		 * (based at 0x00000000)
		 */
		mbox[28] &= 0x3FFFFFFF;

		// Access frame buffer as 1 byte per each address
		fb = (unsigned char*) ((unsigned long) mbox[28]);
		uart_puts("Got allocated Frame Buffer at RAM physical address: ");
		uart_hex(mbox[28]);
		uart_puts("\n");
		uart_puts("Frame Buffer Size (bytes): ");
		uart_dec(mbox[29]);
		uart_puts("\n");
		actual_width = mbox[5]; // Actual physical width
		actual_height = mbox[6]; // Actual physical height
		pitch = mbox[33]; // Number of bytes per line
	} else {
		uart_puts("Unable to get a frame buffer with provided setting\n");
	}
}

void set_screen_size_without_option(int width, int height){
	mbox[0] = 35 * 4; // Length of message in bytes
	mbox[1] = MBOX_REQUEST;
	mbox[2] = MBOX_TAG_SETPHYWH; //Set physical width-height
	mbox[3] = 8; // Value size in bytes
	mbox[4] = 0; // REQUEST CODE = 0
	mbox[5] = width;
	mbox[6] = height;
	mbox[7] = MBOX_TAG_SETVIRTWH; //Set virtual width-height
	mbox[8] = 8;
	mbox[9] = 0;
	mbox[10] = width;
	mbox[11] = height;
	mbox[12] = MBOX_TAG_SETVIRTOFF; //Set virtual offset
	mbox[13] = 8;
	mbox[14] = 0;
	mbox[15] = 0; // x offset
	mbox[16] = 0; // y offset
	mbox[17] = MBOX_TAG_SETDEPTH; //Set color depth
	mbox[18] = 4;
	mbox[19] = 0;
	mbox[20] = COLOR_DEPTH; //Bits per pixel
	mbox[21] = MBOX_TAG_SETPXLORDR; //Set pixel order
	mbox[22] = 4;
	mbox[23] = 0;
	mbox[24] = PIXEL_ORDER;
	mbox[25] = MBOX_TAG_GETFB; //Get frame buffer
	mbox[26] = 8;
	mbox[27] = 0;
	mbox[28] = 16; //alignment in 16 bytes
	mbox[29] = 0; //will return Frame Buffer size in bytes
	mbox[30] = MBOX_TAG_GETPITCH; //Get pitch
	mbox[31] = 4;
	mbox[32] = 0;
	mbox[33] = 0; //Will get pitch value here
	mbox[34] = MBOX_TAG_LAST;

	// Call Mailbox
	if (mbox_call(ADDR(mbox), MBOX_CH_PROP) //mailbox call is successful ?
		&& mbox[20] == COLOR_DEPTH //got correct color depth ?
		&& mbox[24] == PIXEL_ORDER //got correct pixel order ?
		&& mbox[28] != 0 //got a valid address for frame buffer ?
		) {
		/* Convert GPU address to ARM address (clear higher address bits)
		 * Frame Buffer is located in RAM memory, which VideoCore MMU
		 * maps it to bus address space starting at 0xC0000000.
		 * Software accessing RAM directly use physical addresses
		 * (based at 0x00000000)
		 */
		mbox[28] &= 0x3FFFFFFF;

		// Access frame buffer as 1 byte per each address
		fb = (unsigned char*) ((unsigned long) mbox[28]);
		uart_puts("Set equal physical and virtual screensize.\n");
		uart_puts("Got allocated Frame Buffer at RAM physical address: ");
		uart_hex(mbox[28]);
		uart_puts("\n");
		uart_puts("Frame Buffer Size (bytes): ");
		uart_dec(mbox[29]);
		uart_puts("\n");
		actual_width = mbox[5]; // Actual physical width
		actual_height = mbox[6]; // Actual physical height
		pitch = mbox[33]; // Number of bytes per line
	} else {
		uart_puts("Unable to get a frame buffer with provided setting\n");
	}
}

void handle_mbox_command(){
    // mailbox data buffer: Read ARM frequency
	mbox[0] = 8*4;          // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
	mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
	mbox[2] = 0x00030002;   // TAG Identifier: Get clock rate
	mbox[3] = 8;            // Value buffer size in bytes (max of request and response lengths)
	mbox[4] = 0;			// REQUEST CODE = 0
	mbox[5] = 3;			// clock id: ARM system clock
	mbox[6] = 0;            // clear output buffer (response data are mbox[5] & mbox[6])
	mbox[7] = MBOX_TAG_LAST;

	if (mbox_call(ADDR(mbox), MBOX_CH_PROP)) {
		uart_puts("Response Code for whole message: ");
		uart_hex(mbox[1]);
		uart_puts("\n");

		uart_puts("Response Code in Message TAG: ");
		uart_hex(mbox[4]);
		uart_puts("\n");

		uart_puts("DATA: ARM clock rate = ");
		uart_dec(mbox[6]);
		uart_puts("\n\n");
	} else {
		uart_puts("Unable to query!\n");
	}


	//Read Board Revision
	mbox[0] = 7*4;          // Message Buffer Size in bytes (9 elements * 4 bytes (32 bit) each)
	mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
	mbox[2] = 0x00010002;   // TAG Identifier: Get clock rate
	mbox[3] = 4;            // Value buffer size in bytes (max of request and response lengths)
	mbox[4] = 0;			// REQUEST CODE = 0
	mbox[5] = 0;            // clear output buffer
	mbox[6] = MBOX_TAG_LAST;

	if (mbox_call(ADDR(mbox), MBOX_CH_PROP)) {
		uart_puts("Response Code for whole message: ");
		uart_hex(mbox[1]);
		uart_puts("\n");

		uart_puts("Response Code in Message TAG: ");
		uart_hex(mbox[4]);
		uart_puts("\n");

		uart_puts("DATA: board revision = ");
		uart_hex(mbox[5]);
		uart_puts("\n\n");

	} else {
		uart_puts("Unable to query!\n");
	}

	//Read Board Model
	mbox[0] = 7*4;          // Message Buffer Size in bytes (4 elements * 4 bytes (32 bit) each)
	mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
	mbox[2] = 0x00010001;   // TAG Identifier: Get board model
	mbox[3] = 4;            // Value buffer size in bytes (max of request and response lengths)
	mbox[4] = 0;			// REQUEST CODE = 0
	mbox[5] = 0;            // clear output buffer
	mbox[6] = MBOX_TAG_LAST;

	if (mbox_call(ADDR(mbox), MBOX_CH_PROP)) {
		uart_puts("Response Code for whole message: ");
		uart_hex(mbox[1]);
		uart_puts("\n");

		uart_puts("Response Code in Message TAG: ");
		uart_hex(mbox[4]);
		uart_puts("\n");

		uart_puts("DATA: board model = ");
		uart_hex(mbox[5]);
		uart_puts("\n\n");

	} else {
		uart_puts("Unable to query!\n");
	}
}

void drawPixelARGB32(int x, int y, unsigned int attr) {
	int offs = (y * pitch) + (COLOR_DEPTH / 8 * x);
	/* //Access and assign each byte
	 *(fb + offs ) = (attr >> 0 ) & 0xFF; //BLUE
	 *(fb + offs + 1) = (attr >> 8 ) & 0xFF; //GREEN
	 *(fb + offs + 2) = (attr >> 16) & 0xFF; //RED
	 *(fb + offs + 3) = (attr >> 24) & 0xFF; //ALPHA
	 */
//Access 32-bit together
	*((unsigned int*) (fb + offs)) = attr;
}

void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill) {
	for (int y = y1; y <= y2; y++)
		for (int x = x1; x <= x2; x++) {
			if ((x == x1 || x == x2) || (y == y1 || y == y2))
				drawPixelARGB32(x, y, attr);
			else if (fill)
				drawPixelARGB32(x, y, attr);
	}
}

void draw_pattern(){
	// This is the list of colors to be used in the pattern
	int grey = 0x00B4B4B4;
	int yellow = 0x00FDF944;
	int cyan = 0x0000FFFF;
	int green = 0x0000FF40;
	int pink = 0x00FF80FF;
	int red = 0x00FF0000;
	int blue = 0x000000FF;
	int orange = 0x00FF8040;
	int color_arr[] = {grey, yellow, cyan, green, pink, red, blue, orange};

	int big_bar_x1 = 10;
	int big_bar_y1 = 10;
	int big_bar_x2 = 100;
	int big_bar_y2 = 350;
	
	int small_bar_x1 = 10;
	int small_bar_y1 = 350;
	int small_bar_x2 = 100;
	int small_bar_y2 = 400;

	// For loop to draw the pattern
	for (int i = 0; i < 8; i++){
		drawRectARGB32(big_bar_x1 + 90*i , big_bar_y1, big_bar_x2 + 90*i, big_bar_y2, color_arr[i], 1);
		drawRectARGB32(small_bar_x1 + 90*i, small_bar_y1, small_bar_x2 + 90*i, small_bar_y2, color_arr[7 - i], 1);
	}

	// Print message on completion
	printf("Pattern drawn.\n");
}