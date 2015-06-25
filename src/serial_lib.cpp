#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int init_sci( char *sci_fd_name){
  struct termios tio;
  int sci_fd;
  
  //~ printf("open sci:[%s]\n",sci_fd_name);
  if ((sci_fd = open(sci_fd_name, O_RDWR | O_NOCTTY | O_NONBLOCK))<0) {
    printf("sci open failed.\n");
    exit(-1);
  }
  memset(&tio,0x00,sizeof(tio));
  tio.c_cflag = B115200 | CRTSCTS | CS8 | CLOCAL | CREAD;
  tio.c_iflag =  IUTF8;
  tio.c_oflag = 0;
  tio.c_lflag = 0;
  tio.c_cc[VTIME] = 0;
  tio.c_cc[VMIN] = 1;
  
  tcflush(sci_fd, TCIFLUSH);
  tcsetattr(sci_fd, TCSANOW, &tio);
  fcntl(sci_fd, F_SETFL, FNDELAY);
  
  return sci_fd;
}


int close_sci(int sci_fd){
  close(sci_fd);
  return 0;
}


int get_sci(int scifd, char *get_char){
  int retval=0;

  while((retval = read(scifd, get_char, 1))<0) {
    switch (errno) {
      case EAGAIN :
      break;
      default :
        perror(strerror(errno));
        return -1;
    }
  }

  return retval;
}

int str_get_sci(int scifd, char *get_str,unsigned int str_len){
  char tempbuff;
  int retval=0,loopcnt=0;

  do{
      get_sci(scifd,&tempbuff);
     if (loopcnt >1) {
        if (tempbuff == '\n') {
          *get_str = '\0';
          //~ return retval;
        }
        else if (tempbuff == '-') {
          *get_str = ' ';
          get_str++;
          *get_str = tempbuff;
          get_str++;
        }
        else {
          *get_str = tempbuff;
          get_str++;
        }
      }
    loopcnt++;
  } while(tempbuff != '\n');
  return retval;
}

int str_put_sci(int scifd,  char *put_str,unsigned int str_len){
  int retval=0,loopcnt;
  for(loopcnt = 0; loopcnt<str_len; loopcnt++) {
    retval = write(scifd,put_str,1);
    if (retval < 0) {
      break;
    }
    put_str++;
  }
  return retval;
}
