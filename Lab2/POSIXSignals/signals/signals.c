#include <stdio.h>     /* standard I/O functions                         */
#include <stdlib.h>    /* exit                                           */
#include <string.h>    /* memset                                         */
#include <unistd.h>    /* standard unix functions, like getpid()         */
#include <signal.h>    /* signal name macros, and the signal() prototype */

/* first, define the Ctrl-C counter, initialize it with zero. */
int ctrl_c_count = 0;
int got_response = 0;
#define CTRL_C_THRESHOLD  5 

/* the Ctrl-C signal handler */
void catch_int(int sig_num)
{
  /* increase count, and check if threshold was reached */
  ctrl_c_count++;
  
  if (ctrl_c_count >= CTRL_C_THRESHOLD) {
    char answer[30];

    /* prompt the user to tell us if to really
     * exit or not */
    printf("\nReally exit? [Y/n]: ");
    fflush(stdout);
    alarm(5);
    fgets(answer, sizeof(answer), stdin);
    if (answer[0] == 'n' || answer[0] == 'N') {
      printf("\nContinuing\n");
      fflush(stdout);
      got_response = 1;
      /* 
       * Reset Ctrl-C counter
       */
      ctrl_c_count = 0;
    }
    else {
      printf("\nExiting...\n");
      fflush(stdout);
      exit(0);
    }
  }
}

/* the Ctrl-Z signal handler */
void catch_tstp(int sig_num)
{
  /* print the current Ctrl-C counter */
  printf("\n\nSo far, '%d' Ctrl-C presses were counted\n\n", ctrl_c_count);
  fflush(stdout);
}

/* STEP - 1 (20 points) */
/* Implement alarm handler - following catch_int and catch_tstp signal handlers */
/* If the user DOES NOT RESPOND before the alarm time elapses, the program should exit */
/* If the user RESPONDEDS before the alarm time elapses, the alarm should be cancelled */
//YOUR CODE
void catch_alarm(int sig_num){
  if(got_response == 0){
  
  printf("\nUser taking too long to respond. Exiting...\n");
  fflush(stdout);
  exit(0);
  }
  else{
  got_response = 0;
  }
}
int main(int argc, char* argv[])
{
  struct sigaction sa;
  
  /* STEP - 2 (10 points) */
  /* clear the memory at sa - by filling up the memory location at sa with the value 0 till the size of sa, using the function memset */
  /* type "man memset" on the terminal and take reference from it */
  /* if the sa memory location is reset this way, then no garbage value can create undefined behavior with the signal handlers */
  //YOUR CODE
  memset(&sa, 0, sizeof(sa));

  sigset_t mask_set;  /* used to set a signal masking set. */
  

  /* STEP - 3 (10 points) */
  /* setup mask_set - fill up the mask_set with all the signals to block*/
  //YOUR CODE
  sigfillset(&mask_set);
  
  /* STEP - 4 (10 points) */
  /* ensure in the mask_set that the alarm signal does not get blocked while in another signal handler */
  //YOUR CODE
  // SIGALRM = 14
  // sigdelset(&mask_set, SIGALRM);
  sigdelset(&mask_set, 14);
  
  /* STEP - 5 (20 points) */
  /* set signal handlers for SIGINT, SIGTSTP and SIGALRM */
  //YOUR CODE
  sa.sa_mask = mask_set;
  
  // SIGINT
  // sigaction(SIGINT, &sa, NULL);
  // SIGINT = 2
  sa.sa_handler = &catch_int;
  sigaction(2, &sa, NULL);
  
  // SIGTSTP
  // sigaction(SIGTSTP, &sa, NULL);
  // SIGTSTP = 18,20,24
  sa.sa_handler = &catch_tstp;
  sigaction(18, &sa, NULL);
  sigaction(20, &sa, NULL);
  sigaction(24, &sa, NULL);
  
  //SIGALRM
  // SIGALRM = 14
  // sigaction(SIGALRM, &sa, NULL);
  sa.sa_handler = &catch_alarm;
  sigaction(14, &sa, NULL);
  
  /* STEP - 6 (10 points) ls
  */
  /* ensure that the program keeps running to receive the signals */
  //YOUR CODE
  while(1){
    pause();
  }

  return 0;
}

