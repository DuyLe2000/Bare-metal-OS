/**
 * This file will be used to handle the commands and request from the user input
 */ 

#include "handler.h"
#include "string.h"
#include "function_handler.h"

void print_command_list(){
    // 1a
    printf("help:\n");
    printf("    Show brief information of all commands.\n");

    // 1b
    printf("help <command_name>:\n");
    printf("    Show full information of the command.\n");

    // 2
    printf("setcolor -t <text color> -b <background color>:\n");
    printf("    Set text color, and/or background color of the console\n");

    // 3
    printf("cls:\n");
    printf("    Clear screen.\n");

    // 4
    printf("brdrev:\n");
    printf("    Show board revision.\n");

    // 5
    printf("scrsize <options>:\n");
    printf("    Set screen size.\n");

    // 6
    printf("draw:\n");
    printf("    Draw on the screen.\n");

    // 7
    printf("brdmod:\n");
    printf("    Get board model.\n");

    // 8
    printf("virsize:\n");
    printf("    Get virtual screen size.\n");

    // 9
    printf("armmem:\n");
    printf("    Get ARM memory.\n");

    // 10
    printf("macadd:\n");
    printf("    Get MAC address.\n");
}

void handle_help_command(char *command){
    // 2
    if (compare_string(command, "setcolor")){
        printf("setcolor -t <text color>\n");
        printf("or setcolor -b <background color>\n");
        printf("    Set text color, and/or background color of the console\n");
        printf("    to one of the following color: BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE\n");
        printf("    Use setcolor -r to reset text and background color to default\n");
    }

    // 3
    else if (compare_string(command, "cls")){
        printf("cls:\n");
        printf("    Clear screen.\n");
        printf("    In our terminal it will scroll down to current position of the cursor.\n");
    }

    // 4 
    else if (compare_string(command, "brdrev")){
        printf("brdrev:\n");
        printf("    Show board revision.\n");
    }

    // 5
    else if (compare_string(command, "scrsize")){
        printf("scrsize:\n");
        printf("    scrsize <options>\n");
        printf("    Set Screen Size.\n");
        printf("    Set physical screen size (-p) or virtual screen size (-v), or both (by default)\n");
        printf("    Example: scrsize -p 1024 768\n");
    }

    // 6 
    else if (compare_string(command, "draw")){
        printf("draw:\n");
        printf("    Draw a colorful pattern on the screen\n");
    }

    // 7
    else if (compare_string(command, "brdmod")){
        printf("brdmod:\n");
        printf("    Get board model\n");
    }

    // 8 
    else if (compare_string(command, "virsize")){
        printf("virsize:\n");
        printf("    Get virtual screen size\n");
    }

    // 9 
    else if (compare_string(command, "armmem")){
        printf("armmem:\n");
        printf("    Get ARM memory of the board\n");
    }

    // 10
    else if (compare_string(command, "macadd")){
        printf("macadd:\n");
        printf("    Get MAC address of the board\n");
    }

    else {
        printf("ERROR: Unrecognized command\n");
    }
}

void handle_input(char child_strings[10][10]){
    // 1a - single help command
    if (compare_string(child_strings[0], "help") && compare_string(child_strings[1], "\0")){
        print_command_list();
    } 

    // 1b - help with another command
    else if (compare_string(child_strings[0], "help") && 
            (compare_string(child_strings[1], "\0") == 0)){
        handle_help_command(child_strings[1]);
    }

    // 2 - Set color/background color for console
    else if (compare_string(child_strings[0], "setcolor")){
        // For set text or background color
        if (compare_string(child_strings[1], "-t") || compare_string(child_strings[1], "-b")){
            set_color(child_strings[1], child_strings[2]);
        }

        //For reset colors
        else if (compare_string(child_strings[1], "-r")){
            if (!(compare_string(child_strings[2], "\0"))){
                printf("ERROR: This command does not require any arguments. Please enter it again.\n");
            } else {
                reset_color();
            }
        }

        // For wrong command format
        else {
            printf("ERROR: Wrong format.\n");
            printf("Please enter setcolor -t <color> or setcolor -b <color> or setcolor -r.\n");
        }
    }

    // 3 - Clear screen command
    else if (compare_string(child_strings[0], "cls")){
        if (!(compare_string(child_strings[1], "\0"))){
            printf("ERROR: This command does not require any arguments. Please enter it again.\n");
        } else {
            clear_screen();
        }
    }

    // 4 - Get board revision
    else if (compare_string(child_strings[0], "brdrev")){
        if (!(compare_string(child_strings[1], "\0"))){
            printf("ERROR: This command does not require any arguments. Please enter it again.\n");
        } else {
            get_board_revision();
        }
    }

    // 5 - Set screen size
    else if (compare_string(child_strings[0], "scrsize")){
        // For the default case of physical = virtual
        if (compare_string(child_strings[3], "\0")){
            // Check to see if both width and height are given 
            if ((compare_string(child_strings[1], "\0")) || (compare_string(child_strings[2], "\0"))){
                printf("ERROR: Not enough arguments.\n");
                printf("Please enter both width and height.\n");
            }
            // Check to see if width and height are correct integers
            else if (check_int(child_strings[1]) && check_int(child_strings[2])){
                int width = atoi(child_strings[1]);
                int height = atoi(child_strings[2]);
                set_screen_size_without_option(width, height);
            } 
            else {
                printf("ERROR: Width and height must be integers.\n");
                printf("Please enter again.\n");
            }            
        }
        
        // For the option to set physical or virtual 
        else if (compare_string(child_strings[1], "-p") || compare_string(child_strings[1], "-v")){
            // Check to see if both width and height are given 
            if ((compare_string(child_strings[2], "\0")) || (compare_string(child_strings[3], "\0"))){
                printf("ERROR: Not enough arguments.\n");
                printf("Please enter both width and height.\n");
            }
            // Check to see if width and height are correct integers
            else if (check_int(child_strings[2]) && check_int(child_strings[3])){
                int width = atoi(child_strings[2]);
                int height = atoi(child_strings[3]);
                set_screen_size_with_option(child_strings[1], width, height);
            } 
            else {
                printf("ERROR: width and height must be integers.\n");
                printf("Please enter again.\n");
            }
        }

        // For wrong command format
        else {
            printf("ERROR; Wrong format.\n");
            printf("Please enter scrsize <width> <height> or\n");
            printf("scrsize -p <width> <height> or scrsize -v <width> <height>\n");
        }
    }

    // 6 - Draw pattern on the screen
    else if (compare_string(child_strings[0], "draw")){
        if (!(compare_string(child_strings[1], "\0"))){
            printf("ERROR: This command does not require any arguments. Please enter it again.\n");
        } else {
            draw_pattern();
        }
    }

    // 7 - Get board model 
    else if (compare_string(child_strings[0], "brdmod")){
        if (!(compare_string(child_strings[1], "\0"))){
            printf("ERROR: This command does not require any arguments. Please enter it again.\n");
        } else {
            get_board_model();
        }
    }

    // 8 - Get virtual screen size
    else if (compare_string(child_strings[0], "virsize")){
        if (!(compare_string(child_strings[1], "\0"))){
            printf("ERROR: This command does not require any arguments. Please enter it again.\n");
        } else {
            get_virtual_screen_size();
        }
    }

    // 9 - Get ARM memory
    else if (compare_string(child_strings[0], "armmem")){
        if (!(compare_string(child_strings[1], "\0"))){
            printf("ERROR: This command does not require any arguments. Please enter it again.\n");
        } else {
            get_arm_memory();
        }
    }

    // 10 - Get MAC address
    else if (compare_string(child_strings[0], "macadd")){
        if (!(compare_string(child_strings[1], "\0"))){
            printf("ERROR: This command does not require any arguments. Please enter it again.\n");
        } else {
            get_mac_address();
        }
    }

    // When no command satisfies the input
    else {
        printf("ERROR: Unrecognized command\n");
    }
}