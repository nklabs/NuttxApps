/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#include <semaphore.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define STACKSIZE 1024
#define PRIORITY   100
/****************************************************************************
 * Private Data
 ****************************************************************************/
static sem_t sem;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
 void task1(void)
 {

  int status, value;
  sleep(3);
  status = sem_getvalue(&sem, &value);

  if (status < 0)
    {
      printf("task1: ERROR thread %d could not get semaphore value\n");
    }
  else
    {
      printf("task1: Thread semaphore value = %d\n", value);
    }

  printf("task1: Thread posting semaphore\n");
  
  status = sem_post(&sem);
  
  if (status != 0)
    {
      printf("task1: ERROR thread sem_wait failed\n");
    }
   
 }

 void task2(void)
 {
  int status, value;

  status = sem_getvalue(&sem, &value);
  
  if (status < 0)
    {
      printf("task2: ERROR thread could not get semaphore value\n");
    }
  else
    {
      printf("task2: Thread initial semaphore value = %d\n", value);
    }

  printf("task2: Thread waiting on semaphore\n");

  status = sem_wait(&sem);

  if (status != 0)
    {
      printf("task2: ERROR thread sem_wait failed\n");
    }

  printf("task2: Thread awakened\n");

 }
 

void task_test(void)
{
  int ret;
  pid_t pid;

  ret = sem_init(&sem, 0, 0);
  if (ret < 0)
    {
      printf("main: sem_init() ERROR.\n");
    }

  /* Start the task */
  ret = task_create("task1", PRIORITY, STACKSIZE, task1, 0);
  if (ret < 0)
    {
      printf("main: ERROR Failed to start task1\n");
    }
  else
    {
      pid = ret;
      printf("main: Started task1 at PID=%d\n", pid);
    }

  /* Start the task */
  ret = task_create("task2", PRIORITY, STACKSIZE, task2, 0);
  if (ret < 0)
    {
      printf("main: ERROR Failed to start task1\n");
    }
  else
    {
      pid = ret;
      printf("main: Started task2 at PID=%d\n", pid);
    }

  printf("main: Exiting\n");
}

int lab1_main(int argc, char *argv[])
{
  task_test();
  return 0;
}