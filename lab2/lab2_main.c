/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <pthread.h>
#include <nuttx/mqueue.h>
#include <sched.h>
#include <errno.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define STACKSIZE 1024
#define PRIORITY   100
#define PAYLOAD "payload"
#define PAYLOAD_LEN ( strlen(PAYLOAD)+1 )
/****************************************************************************
 * Private Data
 ****************************************************************************/
static mqd_t sendq;
static mqd_t recvq;
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * NK Labs: Lab1 
 ****************************************************************************/

void task3(void)
{
    struct mq_attr attr;
    int status = 0;

    printf("sender_thread: Starting\n");

    /* Fill in attributes for message queue */

    attr.mq_maxmsg  = 1;
    attr.mq_msgsize = PAYLOAD_LEN;
    attr.mq_flags   = 0;

    sendq = mq_open("mqueue", O_RDWR|O_CREAT, 0666, &attr);
    if (sendq == (mqd_t)-1)
    {
      printf("sender_thread: ERROR mq_open failed\n");
      pthread_exit((pthread_addr_t)1);
    }

    sleep(1);
    status = mq_send(sendq, PAYLOAD, PAYLOAD_LEN, 42);
    if (status < 0)
    {
      printf("sender_thread: ERROR mq_send failure=%d on msg %d\n", status);
    }
    else
    {
      printf("sender_thread: mq_send succeeded on msg %d\n");
    }

}

void task4(void)
{
    struct mq_attr attr;
    char msg_buffer[32];
    int nbytes = 0;
    int i = 0;
    memset(msg_buffer, 0x00, PAYLOAD_LEN);
    printf("sender_thread: Starting\n");
    //msg_buffer[0] = 'd';
    attr.mq_maxmsg  = 1;
    attr.mq_msgsize = PAYLOAD_LEN;
    attr.mq_flags   = 0;

    recvq = mq_open("mqueue", O_RDWR|O_CREAT, 0666, &attr);
    if(sendq == (mqd_t)-1)
    {
      printf("sender_thread: ERROR mq_open failed\n");
    }

    nbytes = mq_receive(recvq, msg_buffer, PAYLOAD_LEN, 0);
    if (nbytes < 0)
    {
        if (errno != EINTR)
        {
            printf("receiver_thread: ERROR mq_receive failure on msg %d, errno=%d\n", errno);
        }
        else
        {
            printf("receiver_thread: mq_receive interrupted!\n");
        }
    }
    else if (nbytes != strlen("payload")+1)
    {
          printf("receiver_thread: mq_receive return bad size %d on msg %d\n", nbytes);
    }
    else
    {
        printf("receiver_thread: mq_receive succeeded on msg %d\n");
        printf("MSG = [");
        while( *(msg_buffer+i) )
        {
          printf("%c", msg_buffer[i++]);

        }
        printf("]\n");
    }
}

void start_tasks(void)
{
   pid_t pid;
   int ret;
   /* Start the task */
  ret = task_create("task3", PRIORITY, STACKSIZE, task3, 0);
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
  ret = task_create("task4", PRIORITY, STACKSIZE, task4, 0);
  if (ret < 0)
    {
      printf("main: ERROR Failed to start task1\n");
    }
  else
    {
      pid = ret;
      printf("main: Started task2 at PID=%d\n", pid);
    }
}

int lab2_main(int argc, char *argv[])
{
  start_tasks();
  return 0;
}
