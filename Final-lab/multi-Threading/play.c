#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>

void judgefun(int *);

void generatenum(int *);
pthread_mutex_t all_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t team1_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t team2_mutex = PTHREAD_MUTEX_INITIALIZER;
int r1 = 50, r2 = 20, r3 = 10;
int p;
int numofpoints = 4;
int numofroundstatic = 6;
int numofround = 6;
int sumteam1 = 0;
int sumteam2 = 0;
int team1point = 0;
int team2point = 0;
int numplayer[4][2];
int rangeteam1 = 10;
int rangeteam2 = 10;
bool cheakgeneratenum[4] = {false, false, false, false};
pthread_t players[4], judge;
int main(int argc, char **argv)
{
  pthread_t player1, player2, player3, player4;
  srand(getpid());
  p = 1;

  for (int i = 0; i < 4; i++)
  {
    numplayer[i][0] = i + 1;

    if ((i + 1) % 2 == 0)
    {
      numplayer[i][1] = 2;
    }
    else
    {
      numplayer[i][1] = 1;
    }
    pthread_create(&players[i], NULL, (void *)generatenum, (void *)&numplayer[i]);
  }

  pthread_create(&judge, NULL, (void *)judgefun, (void *)&r1);

  for (int i = 0; i < 4; i++)
  {
    pthread_join(players[i], NULL);
  }

  pthread_join(judge, NULL);

  pthread_mutex_destroy(&team1_mutex);
  pthread_mutex_destroy(&team2_mutex);
  pthread_mutex_destroy(&all_mutex);
  return 0;
}

void generatenum(int *mydata)
{

  int team = 0;
  while (1)
  {
    pthread_mutex_lock(&all_mutex);
    for (int i = 0; i < 4; i++)
    {
      if (pthread_equal(players[i], pthread_self()))
      {
        team = numplayer[i][1];
        break;
      }
    }

    pthread_mutex_unlock(&all_mutex);

    if (team == 1)
    {

      int Randomnum;
      int n = 5;

      pthread_mutex_lock(&team1_mutex);
      if (p == 0)
      {
        pthread_mutex_unlock(&team1_mutex);
        exit(0);
      }
      else if (p != 0 && cheakgeneratenum[mydata[0] - 1] == false)
      {

        Randomnum = rand() % rangeteam1;
        Randomnum = Randomnum + 1;

        sumteam1 = sumteam1 + Randomnum;

        printf("team1 my num= %d my team= %d  random num=%d  \n\n", mydata[0], mydata[1], Randomnum);

        cheakgeneratenum[mydata[0] - 1] = true;

        n = n - 1;
      }

      pthread_mutex_unlock(&team1_mutex);
    }

    ///**********************************//
    else if (team == 2)
    {

      int Randomnum;
      int n = 5;
      pthread_mutex_lock(&team2_mutex);
      if (p == 0)
      {
        pthread_mutex_unlock(&team2_mutex);
        exit(0);
      }
      else if (p != 0 && cheakgeneratenum[mydata[0] - 1] == false)
      {

        Randomnum = rand() % rangeteam2;
        Randomnum = Randomnum + 1;
        sumteam2 = sumteam2 + Randomnum;

        printf("team2 my num= %d my team= %d  random num=%d \n\n", mydata[0], mydata[1], Randomnum);

        cheakgeneratenum[mydata[0] - 1] = true;

        n = n - 1;
      }

      pthread_mutex_unlock(&team2_mutex);
 
    }
  }

  sleep(3);
}

void judgefun(int *num)
{
  int count = 0;
  while (1)
  {

    pthread_mutex_lock(&all_mutex);
    pthread_mutex_lock(&team1_mutex);
    pthread_mutex_lock(&team2_mutex);
    if (numofround == 0)
    {
    }

    else
    {

      count = 0;
      for (int i = 0; i < 4; i++)
      {

        if (cheakgeneratenum[i] == true)
        {
          count = count + 1;
        }
      }
      if (count == 4)
      {

        if (sumteam1 > sumteam2)
        {

          if (rangeteam1 != 10)
          {

            rangeteam1 = rangeteam1 / 2;
          }

          rangeteam2 = rangeteam2 * 2;
          team1point = team1point + 1;
          if (numplayer[1][1] == 2)
          {
            numplayer[1][1] = 1;
          }

          if (numplayer[2][1] == 2)
          {

            numplayer[2][1] = 1;
          }
        }
        else if (sumteam2 > sumteam1)
        {
          team2point = team2point + 1;
          if (rangeteam2 != 10)
          {

            rangeteam2 = rangeteam2 / 2;
          }

          rangeteam1 = rangeteam1 * 2;

          if (numplayer[1][1] == 1)
          {
            numplayer[1][1] = 2;
          }

          if (numplayer[2][1] == 1)
          {
            numplayer[2][1] = 2;
          }
        }
        for (int i = 0; i < 4; i++)
        {
          cheakgeneratenum[i] = false;
        }
        printf("****** round number = %d team1 sum = %d team2 sum = %d  team1 point = %d  team2 point = %d\n\n", numofroundstatic - numofround + 1, sumteam1, sumteam2, team1point, team2point);
        numofround = numofround - 1;
        sumteam1 = 0;
        sumteam2 = 0;

        if (numofround == 0 || team1point == numofpoints || team2point == numofpoints)
        {

          p = 0;
          pthread_mutex_unlock(&team2_mutex);
          pthread_mutex_unlock(&team1_mutex);

          printf(" the game has finished with number of round = %d \n", numofroundstatic - numofround);
          if (team1point > team2point)
          {
            printf(" the team1 own\n");
          }
          else if (team1point < team2point)
          {
            printf(" the team2 own\n");
          }
          else
          {
            printf("******* equality *******\n");
          }
          exit(0);
        }
      }
    }
    pthread_mutex_unlock(&team1_mutex);
    pthread_mutex_unlock(&team2_mutex);
    pthread_mutex_unlock(&all_mutex);
  }
}
