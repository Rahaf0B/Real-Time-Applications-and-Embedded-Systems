#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#define KEY 500
#define PUBLIC_PORT 1
int m, n, fd, m1;
int pid_player[4][2];
int sumteam1 = 0;
int sumteam2 = 0;
int team1point = 0;
int team2point = 0;
int roundnum = 4;
int point = 5;
int rangteam1 = 10;
int rangteam2 = 10;
int orginaleange = 10;
int userround = 4;
void recivemsg();

struct msg
{
	long int type;
	int num; // actual message
	int pid; // to know to which client to return the file data
	int teamnum;
} p, q;

struct option
{
	long int type;
	// int team[2];
	int teamnum;
	int rang;

} op;

void signal_handler(int signal)
{
	recivemsg();
}

void recivemsg()
{

	ssize_t bytes_received;
	int numberofmsg = 0;
	bytes_received = msgrcv(m, &p, sizeof(p) - 4, 11, 0);
	int restart = 0;
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

		numberofmsg = numberofmsg + 1;
		printf("number received from child: %d child pid => %d  team number => %d\n", p.num, p.pid, p.teamnum);
		fflush(stdout);
		printf("\n\n\n");
		fflush(stdout);
	}
}

void allmassages()
{
	ssize_t bytes_received;
	int numberofmsg = 0;
	while (bytes_received = msgrcv(m, &p, sizeof(p) - 4, 30, 0))
	{
		int restart = 0;
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

			numberofmsg = numberofmsg + 1;
			printf("number received from child: %d child pid => %d  team number => %d\n", p.num, p.pid, p.teamnum);
			printf("\n\n\n");
			fflush(stdout);

			if (p.teamnum == 1)
			{

				sumteam1 = sumteam1 + p.num;
			}

			else
			{
				sumteam2 = sumteam2 + p.num;
			}
		}
		if (numberofmsg == 4)
		{
			break;
		}
	}

	printf("sum team1 == %d sum team2 == %d \n\n", sumteam1, sumteam2);
	fflush(stdout);
	sleep(2);

	fflush(stdout);
	if (sumteam1 > sumteam2)
	{

		team1point = team1point + 1;
		if (rangteam1 > orginaleange)
		{
			rangteam1 = rangteam1 / 2;
		}

		op.rang = rangteam1;
		op.teamnum = 1;
		rangteam2 = rangteam2 * 2;
		if (pid_player[1][1] != 1)
		{
			pid_player[1][1] = 1;
		}
		if (pid_player[2][1] == 2)
		{
			pid_player[2][1] = 1;
		}

		for (int j = 0; j < 4; j++)
		{
			if (pid_player[j][1] == 1)
			{
				op.type = pid_player[j][0];
				msgsnd(m, &op, sizeof(op) - 4, 0);
				kill(pid_player[j][0], SIGUSR1);
			}
		}

		sleep(2);

		op.rang = rangteam2;
		op.teamnum = 2;

		for (int j = 0; j < 4; j++)
		{
			if (pid_player[j][1] == 2)
			{

				op.type = pid_player[j][0];

				msgsnd(m, &op, sizeof(op) - 4, 0);
				kill(pid_player[j][0], SIGUSR1);
			}
		}
		sleep(2);
	}

	else if (sumteam1 < sumteam2)
	{

		if (rangteam2 > orginaleange)
		{
			rangteam2 = rangteam2 / 2;
		}

		op.rang = rangteam2;
		op.teamnum = 2;
		rangteam1 = rangteam1 * 2;
		team2point = team2point + 1;
		if (pid_player[2][1] != 2)
		{

			pid_player[2][1] = 2;
			sleep(2);
		}

		if (pid_player[1][1] == 1)
		{

			sleep(2);
			pid_player[1][1] = 2;
		}

		for (int j = 0; j < 4; j++)
		{
			if (pid_player[j][1] == 2)
			{
				op.type = pid_player[j][0];
				msgsnd(m, &op, sizeof(op) - 4, 0);
				kill(pid_player[j][0], SIGUSR1);
			}
		}

		sleep(2);

		op.rang = rangteam1;
		op.teamnum = 1;

		for (int j = 0; j < 4; j++)
		{
			if (pid_player[j][1] == 1)
			{

				op.type = pid_player[j][0];

				msgsnd(m, &op, sizeof(op) - 4, 0);
				kill(pid_player[j][0], SIGUSR1);
			}
		}
	}

	sleep(2);

	roundnum = roundnum - 1;
}

int main()
{

	m = msgget(KEY, 0666 | IPC_CREAT); // create new queue

	for (int k = 0; k < 4; k++)
	{
		if ((k + 1) % 2 == 0)
		{

			pid_player[k][1] = 2;
		}
		else
		{
			pid_player[k][1] = 1;
		}
	}
	for (int i = 0; i < 4; i++)
	{

		switch (pid_player[i][0] = fork())
		{
		case -1:
			perror("Fork failure ");
			exit(-3);

		case 0: /* In the child */
			char teamnumber[5];
			char playernum[5];
			sprintf(playernum, "%d", i + 1);

			if ((i + 1) % 2 == 0)
			{
				sprintf(teamnumber, "%d", 2);
			}
			else
			{
				sprintf(teamnumber, "%d", 1);
			}

			sleep(2);
			fflush(stdin);
			execlp("./child", "child", teamnumber, playernum, (char *)NULL);
			perror("Exec failure ");
			return -4;
			break;

		default:

			/* In the parent */
			;
		}
	}

	sleep(5);

	while (roundnum != 0)
	{

		for (int i = 0; i < 4; i++)
		{

			kill(pid_player[i][0], SIGINT);
		}

		if (team1point == point || team2point == point)
		{

			break;
		}
		allmassages();
	}
	printf("team1point===   %d   team2point==== %d\n\n", team1point, team2point);

	printf("The game finished with number of round = %d\n", userround - roundnum);

	if (team1point > team2point)
	{
		printf("team1 own\n");
	}
	else if (team2point > team1point)
	{
		printf("team2 own\n");
	}
	else
	{
	}
	for (int i = 0; i < 4; i++)
	{

		kill(pid_player[i][0], SIGUSR2);
	}

	while (wait(NULL) > 0)
		;
}
