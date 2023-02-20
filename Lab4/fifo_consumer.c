#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "file.fifo"
#define MAX_LENGTH 1000

main()
{
  char str[MAX_LENGTH];
  int num, fd;

  /* create a FIFO special file with name FIFO_NAME */
  mkfifo(FIFO_NAME, 0666);

  /* open the FIFO file for reading. open() blocks for writers. */
  printf("waiting for writers...");
  fflush(stdout);
  fd = open(FIFO_NAME, O_RDONLY);  //open FIFO for reading only

  printf("got a writer !\n");

  do{
    if((num = read(fd, str, MAX_LENGTH)) == -1)
      perror("read");
    else{
      str[num] = '\0';
      printf("consumer: read %d bytes\n", num);
      printf("%s", str);
    }
  }while(num > 0);
}
