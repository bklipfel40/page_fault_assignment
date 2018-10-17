/*
 *
 *
 * CS 441/541: Page Fault Algorithm Simulator (Project 6)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/******************************
 * Defines
 ******************************/
#define TRUE 1
#define FALSE 0
#define MAXLINE 1024

/******************************
 * Structures
 ******************************/
enum States {READ, WRITE};

struct Page_Info {
  int reference;
  int state;
};

/******************************
 * Global Variables
 ******************************/

 // this will be the one read from the file
struct Page_Info ** all_pages;

// The default will be seven unless specificed
int frames_num = 7;

// boolean if they show all the frames
int all_frames;

// this will be the variable to hold the first line of the file.
int num_page_ref;

char * file_name;

int pointer;

/******************************
 * Function declarations
 ******************************/
/*
* This will parse the command line for all the information.
* RETURN: 0 for no errors, -1 for any errors
*/
int parse_command_line(int argc, char **argv);

/*
* This will read the file given in the command line arguments.
*/
void read_file();

/*
* This will print the information for the user
* It will follow this format
* Num. Frames : All
* Ref. File   : given-tests/level1.txt
* -------------------------------
* Reference String:
* 4:R, 1:W, 3:R, 2:W, 4:R
* -------------------------------
* ####################################################
* #       Frames    Opt.    FIFO    LRU     SC     ESC
* #            1       5       5      5      5       5
* #            2       4       5      5      5       5
* #            3       4       5      5      5       5
* #            5       4       4      4      4       4
* #            6       4       4      4      4       4
* #            7       4       4      4      4       4
* ####################################################
*
* If they give a number with a frame than you will only show one frame:
* Num. Frames : 7
* Ref. File   : given-tests/level7.txt
* -------------------------------
* Reference String:
* 4:R, 1:W, 3:R, 2:W, 4:R
* -------------------------------
* ####################################################
* #       Frames    Opt.    FIFO    LRU     SC     ESC
* #            7       4       4      4      4       4
* ####################################################
*/
void print_information();

/*
* This will clean up all the Structures that were created.
*/
void clean_up_memory();

/*********************************
* ALL THE PAGE REPLACEMENT ALOGRITHMS
**********************************/

/*
* This will run the Optimal Algorithm.
*/
int optimal( int num_frames );

/*
* This will run the First In First Out Algorithm.
*/
int first_in_first_out( int num_frames );

/*
* This will run the Least Recently Used Algorithm.
*/
int least_recently_used( int num_frames );

/*
* This will run the Least Recently Used Second Chance Algorithm.
*/
int second_chance( int num_frames );

/*
* This will run the Least REcently Used Enchance Second Chance Algorithm.
*/
int enhanced_second_chance( int num_frames );

/*
* debug FUNCTIONS
*/

void debug_print_pages();

/*
 * Search the stack of memory to see if the number is in it,
 * if it is, return the index, otherwise return -1;
 */
int search_frames( int * frames, int num, int pages );
