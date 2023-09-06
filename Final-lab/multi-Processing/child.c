#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#include <sys/ipc.h>
#include <time.h>

#include <sys/shm.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h> /* Needed for the wait function */
#include <unistd.h>   /* Needed for the fork function */
#include <string.h>   /* Needed for the strcat function */
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdint.h>
#include <sys/mman.h>
#define KEY 500
#define PUBLIC_PORT 1

int m;
int Randomnum;
int teamnumber;
int playernum;
volatile sig_atomic_t signal_exit_received = 0;

int rangnum = 10;
void(*old_handler); // to save the address of past ctrl+c function

struct msg
{
    long int type;
    int num;
    int pid;
    int teamnum;
} p, q;

struct option
{
    long int type;

    int teamnum;
    int rang;

} op;

void signal_handler3(int signal)
{
    printf("my pid %d my teamnum %d\n\n", getpid(), teamnumber);

    ssize_t bytes_received;
    int numberofmsg = 0;
    bytes_received = msgrcv(m, &op, sizeof(op) - 4, getpid(), 0);

    if (bytes_received == -1)
    {
        perror("msgrcv");
        exit(1);
    }
    else if (bytes_received == 0)
    {
        printf("No message received\n");
    }
    else
    {

        rangnum = op.rang;

        teamnumber = op.teamnum;

        printf("*****data from parent pid %d teamnum== %d playernum == %d   range === %d\n\n", getpid(), op.teamnum, playernum, op.rang);
    }
}

void signal_handler2()
{

    signal(SIGINT, old_handler);
    signal_exit_received = 1;
    exit(0);
}

void sendmsg()
{
    Randomnum = rand() % rangnum;
    Randomnum = Randomnum + 1;

    q.num = Randomnum;
    q.teamnum = teamnumber;
    q.type = 30;
    q.pid = getpid();
    printf("pid => %d, team => %d ff %d \n", getpid(), teamnumber, q.teamnum);
    msgsnd(m, &q, sizeof(q) - 4, 0);
}

void signal_handler4()
{
    signal(SIGINT, signal_handler4);
    sendmsg();
}

int main(int argc, char *argv[])
{
    printf("%d === , %s    %s\n", getpid(), argv[1], argv[2]);
    teamnumber = atoi(argv[1]);
    playernum = atoi(argv[2]);
    srand(getpid());
    signal(SIGUSR1, signal_handler3);
    signal(SIGUSR2, signal_handler2);              // Set up the signal handler
    old_handler = signal(SIGINT, signal_handler4); // save the original func
    while (!signal_exit_received)
    {
        pause(); // Wait for the signal from the parent process
    }
}
