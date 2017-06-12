#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/*
root@ubuntu16:~# gcc stack0 -m32 -o stack0_32

root@ubuntu16:~# gcc --version
gcc (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609
root@ubuntu16:~# uname -a
Linux ubuntu16 4.4.0-77-generic #98-Ubuntu SMP Wed Apr 26 08:34:02 UTC 2017 x86_64 x86_64 x86_64 GNU/Linux
*/

int main(int argc, char **argv)
{
  volatile int modified;
  char buffer[64];

  modified = 0;
  gets(buffer);

  if(modified != 0) {
      printf("you have changed the 'modified' variable\n");
  } else {
      printf("Try again?\n");
  }
}
