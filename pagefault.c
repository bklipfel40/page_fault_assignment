/*
*
*
* CS 441/541: Page Fault Algorithm Simulator (Project 6)
*/
#include "pagefault.h"


int main(int argc, char **argv) {
  all_frames = TRUE;
  int ret = parse_command_line(argc, argv);
  if( ret != 0 ) {
    // error
    return -1;
  }

  read_file();

  //debug_print_pages();

  print_information();

  clean_up_memory();
  return 0;
}

int parse_command_line(int argc, char **argv ) {
  int i;

  for( i = 1; i < argc; i++ ) {
    if( strcmp(argv[i], "-f") != 0) {
      file_name = argv[i];
    } else if ( strcmp(argv[i], "-f") == 0 ) {
      if( i+1 < argc ) {
        frames_num = atoi(argv[i+1]);
        i = i+1;
        all_frames = FALSE;
      }
    }
  }

  if( !file_name ) {
    printf("Error: You must supply a file to read!\n");
    return -1;
  } else if( frames_num > 8 || frames_num <= 0 ) {
    printf("Error: Must supply an integer argument between 1 and 7 for the -f option\n");
    return -1;
  }

  return 0;
}

void read_file() {
  FILE *fp;
  char line[MAXLINE];
  char *tokens[2];
  char * split;
  char value;
  int i = 0, j = 0;
  int is_first_line = TRUE;

  fp = fopen(file_name, "r");
  if ( !fp ) {
    printf("Error: Cannot open the file: %s\n", file_name);
    exit(-1);
  }

  while( fgets(line, sizeof(line), fp ) != NULL ) {
    if( is_first_line ) {
      num_page_ref = atoi(line);
      all_pages = calloc(num_page_ref, sizeof(struct Page_Info *));
      is_first_line = FALSE;
    } else {
      struct Page_Info * new = calloc(1,sizeof(struct Page_Info));
      split = strtok (line," ");
      // splitting it into a reference and state (READ or WRITE)
      while ( split != NULL ) {
        tokens[j] = split;
        split = strtok (NULL," ");
        j++;
      }
      value = tokens[1][0];
      new->reference = atoi(tokens[0]);
      if( value == 87 || value == 119 ) {
        new->state = WRITE;
      } else if ( value == 82 || value == 114 ) {
        new->state = READ;
      }
      all_pages[i] = new;
      // reset to j to zero for other lines of the file
      i++;
      j=0;
    }
  }
  fclose(fp);
}

void print_information() {
  int i;
  if( all_frames ) {
    printf("Num. Frames :  All\n");
  } else {
    printf("Num. Frames :  %d\n", frames_num);
  }
  printf("Ref. File   :  %s\n", file_name);
  printf("-------------------------------\n");
  printf("Reference String:\n ");

  if( num_page_ref == 1 ){
    printf("%d:%s \n",all_pages[0]->reference, (all_pages[0]->state == 0 ? "R" : "W"));
  }
  else{
    printf("%d:%s, ",all_pages[0]->reference, (all_pages[0]->state == 0 ? "R" : "W"));
    for( i = 1; i < num_page_ref-1; i++ ){
      if( (i+1)%10 == 0 ){
        printf(" %d:%s,  \n",all_pages[i]->reference, (all_pages[i]->state == 0 ? "R" : "W"));
      }
      else{
        printf(" %d:%s, ",all_pages[i]->reference, (all_pages[i]->state == 0 ? "R" : "W"));
      }
    }
    printf(" %d:%s \n",all_pages[i]->reference, (all_pages[i]->state == 0 ? "R" : "W"));

  }

  printf("-------------------------------\n");
  printf("####################################################\n");
  printf("#     Frames    Opt.    FIFO     LRU      SC     ESC\n");

  if( all_frames ) {
    for( i = 0; i < frames_num; i++ ) {
      printf("#        %3d     %3d     %3d     %3d     %3d     %3d\n", i+1, optimal(i+1),
      first_in_first_out(i+1), least_recently_used(i+1), second_chance(i+1),
      enhanced_second_chance(i+1));
    }
  } else {
    printf("#        %3d     %3d     %3d     %3d     %3d     %3d\n", frames_num, optimal(frames_num),
    first_in_first_out(frames_num), least_recently_used(frames_num), second_chance(frames_num),
    enhanced_second_chance(frames_num));
  }
  printf("####################################################\n");
}

void clean_up_memory() {
  int i;
  for( i = 0; i < num_page_ref; i++ ) {
    struct Page_Info * p = all_pages[i];
    free(p);
  }
  free(all_pages);
}

/*********************************
* ALL THE PAGE REPLACEMENT ALOGRITHMS
**********************************/

int optimal( int num_frames ) {
  //counters
  int i,j,temp;
  //used to see if our frames are full
  int idx = 0;
  int counter = 0;
  pointer = 0;
  //our array of integers to represent the frames
  int * frames = calloc(num_frames, sizeof(int));
  int num_faults = 0;
  //initialize the frames to be all -1
  for( i = 0; i < num_frames; i++ ){
    frames[i] = -1;
  }
  for( i = 0; i < num_page_ref; i++){
    if( search_frames( frames, all_pages[i]->reference, num_frames) < 0 ){
      if( idx < num_frames ){
        frames[idx] = all_pages[i]->reference;
        idx++;
      }
      else{//we could not find the number and the frames is full
        int * copy = calloc(num_frames, sizeof(int));
        for( temp = 0; temp < num_frames; temp++ ){
          copy[temp] = -1;
        }
        for( j = i+1; j < num_page_ref; j++ ){
          temp = search_frames( frames, all_pages[j]->reference, num_frames );
          if( temp >= 0 ){//the future number is in our frames
            if( (search_frames( copy, all_pages[j]->reference, num_frames)) < 0 ){//see if we already encountered it
              pointer = temp;
              copy[counter] = all_pages[j]->reference;
              counter++;
            }
            if( counter == num_frames ){
              j = num_page_ref; //not sure if break was working
            }
          }
        }
        if( counter < num_frames ){
          for( temp = 0; temp < num_frames; temp++ ){
            pointer = search_frames(copy, frames[temp], num_frames);
            if( pointer < 0 ){
              pointer = temp;
              temp = num_frames;//not sure if break was working
            }
          }
        }
        frames[pointer] = all_pages[i]->reference;
        pointer = 0;
        counter = 0;
        free(copy);
      }
      num_faults++;
    }
    //we have found the number, do nothing
  }//we have gone through all information
  free(frames);
  return num_faults;
}

int first_in_first_out( int num_frames ) {
  //counter
  int i;
  //this will check and see if we have filled the frames yet
  int idx = 0;
  //this will be the oldest number to entered (to maintain FIFO)
  pointer = 0;
  //the number of page faults
  int num_faults = 0;
  //the "frames" to keep track of memory
  int * frames = calloc(num_frames, sizeof(int));
  //initialize the frames to be all -1
  for( i = 0; i < num_frames; i++ ){
    frames[i] = -1;
  }
  //go through and read our array of page information
  for( i = 0; i < num_page_ref; i++ ){
    if( search_frames(frames, all_pages[i]->reference, num_frames) < 0 ){
      //couldn't find the number but we still have room in the frames
      if( idx < num_frames ){
        frames[idx] = all_pages[i]->reference;
        idx++;
      }
      else{//the frames is full and we still didn't find it
      //the oldest number becomes the number we read
      frames[pointer] = all_pages[i]->reference;
      //increase the pointer (will always be in a row)
      pointer = (pointer+1)%num_frames;
    }
    num_faults++;
  }
  //if the number is in the frames don't do anything
}
free(frames);
return num_faults;
}

//Basically the same logic as optimal except reverse, utilizing an int * history
//which keeps track of everything we have read so far
int least_recently_used( int num_frames ) {
  int i,j,temp;
  int idx = 0;
  int counter = 0;
  pointer = 0;
  int * frames = calloc(num_frames, sizeof(int));
  int * history = calloc( num_page_ref, sizeof(int));
  int num_faults = 0;
  for( i = 0; i < num_frames; i++ ){
    frames[i] = -1;
  }
  for( i = 0; i < num_page_ref; i++){
    history[i] = all_pages[i]->reference;
    if( search_frames( frames, all_pages[i]->reference, num_frames) < 0 ){
      if( idx < num_frames ){
        frames[idx] = all_pages[i]->reference;
        idx++;
      }
      else{
        int * copy = calloc(num_frames, sizeof(int));
        for( temp = 0; temp < num_frames; temp++ ){
          copy[temp] = -1;
        }
        for( j = i-1; j >= 0; j-- ){
          temp = search_frames( frames, history[j], num_frames );
          if( temp >= 0 ){
            if( (search_frames( copy, history[j], num_frames)) < 0 ){
              pointer = temp;
              copy[counter] = history[j];
              counter++;
            }
            if( counter == num_frames ){
              j = -1;
            }
          }
        }
        if( counter < num_frames ){
          for( temp = 0; temp < num_frames; temp++ ){
            pointer = search_frames(copy, frames[temp], num_frames);
            if( pointer < 0 ){
              pointer = temp;
              temp = num_frames;
            }
          }
        }
        frames[pointer] = all_pages[i]->reference;
        pointer = 0;
        counter = 0;
        free(copy);
      }
      num_faults++;
    }
    //we have found the number, do nothing
  }
  free(history);
  free(frames);
  return num_faults;
}

int second_chance( int num_frames) {
  //counter
  int i,ref;
  pointer = 0;
  //this will check and see if we have filled the frames yet
  int idx = 0;
  //the number of page faults
  int num_faults = 0;
  //the "frames" to keep track of memory
  int * frames = calloc(num_frames, sizeof(int));
  int * bits = calloc( num_frames, sizeof(int));
  //initialize the frames to be all -1
  for( i = 0; i < num_frames; i++ ){
    frames[i] = -1;
    bits[i] = 1;
  }
  //go through and read our array of page information
  for( i = 0; i < num_page_ref; i++ ){
    ref = search_frames(frames, all_pages[i]->reference, num_frames);
    if( ref < 0 ){
      if( idx < num_frames ){//couldn't find the number but we still have room in the frames
        frames[idx] = all_pages[i]->reference;
        //bits[idx] = 1;
        idx++;
      }
      else{//the frames are full and we still didn't find it
      //go through each frame until we find one with a bit of 0
        while( 1 ){
          if( bits[pointer] == 1 ){
            bits[pointer] = 0;
          }
          else if(bits[pointer] == 0){
            break;
          }
          pointer = (pointer+1)%num_frames;
        }
        frames[pointer] = all_pages[i]->reference;
        bits[pointer] = 1;
        pointer = (pointer+1)%num_frames;
      }
      num_faults++;
    }
    else{//number is in memory
      bits[ref] = 1;
    }
  }
  free(frames);
  free(bits);
  return num_faults;
}

int enhanced_second_chance( int num_frames ) {
  //counter
  int i,j,ref;
  pointer = 0;
  //this will check and see if we have filled the frames yet
  int idx = 0;
  int done;
  //the number of page faults
  int num_faults = 0;
  //a temp to keep my pointer on track
  int temp;
  //the "frames" to keep track of memory
  int * frames = calloc(num_frames, sizeof(int));
  int * reference_bit = calloc( num_frames, sizeof(int));
  int * dirty_bit = calloc( num_frames, sizeof(int));
  //initialize the frames to be all -1
  for( i = 0; i < num_frames; i++ ){
    frames[i] = -1;
    dirty_bit[i] = -1;
  }
  /* JUST TO MAKE SURE IM NOT CRAZY
  for( i = 0; i < num_page_ref; i++ ){
    printf("NUMBER : %d\n", all_pages[i]->reference );
  }
  */
  for( i = 0; i < num_page_ref; i++ ){// go through everything
    ref = search_frames(frames, all_pages[i]->reference, num_frames);
    if( ref < 0 ){//the number is not in our frames
      if( idx < num_frames ){//not all frames are full yet
        //printf("REPLACING %d WITH %d AT: %d\n", frames[i], all_pages[i]->reference, idx);
        frames[idx] = all_pages[i]->reference;
        reference_bit[idx] = 1;
        dirty_bit[idx] = all_pages[i]->state;
        idx++;
      }
      else{//the frames are full
        done = 0;
        temp = pointer;
        //first pass, look for (0,0) (don't change reference bit)
        for( j = 0; j < num_frames; j++ ){
          if( reference_bit[pointer] == 0 && dirty_bit[pointer] == 0 ){
            done = 1;
            break;
          }
          pointer = (pointer+1)%(num_frames);
        }
        //second pass, look for (0,1), change reference bit as we pass
        if( done == 0 ){
          pointer = temp;
          for( j = 0; j < num_frames; j++ ){
            if( reference_bit[pointer] == 0 && dirty_bit[pointer] == 1 ){
              done = 1;
              break;
            }
            if( reference_bit[pointer] == 1 ){
              reference_bit[pointer] = 0;
            }
            pointer = (pointer+1)%(num_frames);
          }
        }
        //third pass, look for (0,0) again
        if( done == 0 ){
          pointer = temp;
          for( j = 0; j < num_frames; j++ ){
            if( reference_bit[pointer] == 0 && dirty_bit[pointer] == 0 ){
              done = 1;
              break;
            }
            pointer = (pointer+1)%(num_frames);
          }
        }
        //fourth pass, look for (0,1) again
        if( done == 0 ){
          pointer = temp;
          for( j = 0; j < num_frames; j++ ){
            if( reference_bit[pointer] == 0 && dirty_bit[pointer] == 1 ){
              break;
            }
            pointer = (pointer+1)%(num_frames);
          }
        }
        //do this no matter what loop breaks
        frames[pointer] = all_pages[i]->reference;
        reference_bit[pointer] = 1;
        dirty_bit[pointer] = all_pages[i]->state;
        pointer = (pointer+1)%(num_frames);
      }//else
      num_faults++;
    }
    else if (ref >= 0 ){//we found the number in memory
      reference_bit[ref] = 1;
      if( all_pages[i]->state == 1 ){
        dirty_bit[ref] = 1;
      }
    }
  }
  free(frames);
  free(reference_bit);
  free(dirty_bit);
  return num_faults;
}

int search_frames( int * frames, int num, int pages){
  int i;
  for( i = 0; i < pages; i++ ){
    if( num == frames[i] ){
      return i;
    }
  }
  //return -1 if the number is not in the frames (page fault)
  return -1;
}

/*********************************
* DEBUG FUNCTIONS
**********************************/
void debug_print_pages() {
  int i;
  for( i = 0; i < num_page_ref; i++ ) {
    struct Page_Info * p = all_pages[i];
    printf("[%d]  ref: %d, state: %s\n", i, p->reference, (p->state == 0 ? "READ" : "WRITE") );
  }
}
