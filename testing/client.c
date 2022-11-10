#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVER_QUEUE_NAME "/mq-test-server"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

int main(int argc, char **argv)
{
    char client_queue_name[64];
    mqd_t qd_server, qd_client;

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    if ((qd_client = mq_open(client_queue_name, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1)
    {
        perror("Error: mq_open (client)");
        exit(1);
    }

    char in_buffer[MSG_BUFFER_SIZE];

    printf("Write message: ");
    char temp_buf[10];

    while (fgets(temp_buf, 2, stdin))
    {
        if (mq_send(qd_server, client_queue_name, strlen(client_queue_name) + 1, 0) == -1)
        {
            perror("Client: not able to send message");
            continue;
        }
    }
}