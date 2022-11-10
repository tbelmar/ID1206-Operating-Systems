#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <mqueue.h>

const int MAX_MESSAGE_SIZE = 2048;
const int MAX_MESSAGES = 10;
const char *QUEUE_NAME = "/channel";

int main()
{
    struct mq_attr attr;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MESSAGE_SIZE;

    int pid = fork();

    // child code
    if (pid == 0)
    {
        mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, (S_IRUSR | S_IWUSR), &attr);
        char *message = "insane in the brain";

        mq_send(mq, message, strlen(message) + 1, 0);

        mq_close(mq);
    }
    // parent code
    else if (pid > 0)
    {
        wait(NULL);

        char rcv[MAX_MESSAGE_SIZE];
        mqd_t mq = mq_open(QUEUE_NAME, O_RDONLY);

        mq_receive(mq, rcv, MAX_MESSAGE_SIZE, 0);

        mq_close(mq);
        mq_unlink(QUEUE_NAME);

        printf("%s\n", rcv);
    }
}