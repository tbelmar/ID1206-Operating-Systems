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

int main()
{
    mqd_t qd_server, qd_client;
    long token_number = 1;

    printf("Server started.\n");

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    if ((qd_server = mq_open(SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1)
    {
        perror("Error: mq_open (server)");
        exit(1);
    }

    char out_buffer[MSG_BUFFER_SIZE];
    char in_buffer[MSG_BUFFER_SIZE];

    qd_client = mq_open(in_buffer, O_WRONLY);

    int x = mq_send(qd_client, out_buffer, strlen(out_buffer) + 1, 0);
    printf("Client send code: %d\n", x);
}