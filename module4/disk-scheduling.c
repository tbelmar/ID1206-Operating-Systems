#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NUM_OF_REQUESTS 1000
#define UPPER_BOUND 4999
#define DISK_SIZE 5000

int disk[DISK_SIZE];
int initialHead;

// first come first serve
int fcfs(int *requests, int head)
{
    int numberOfMovements = 0;

    // copy requests into identical array
    int requestsCopy[NUM_OF_REQUESTS];
    for (int i = 0; i < NUM_OF_REQUESTS; i++)
        requestsCopy[i] = requests[i];

    // loop through requests and serve each one as it comes
    for (int i = 0; i < NUM_OF_REQUESTS; i++)
    {
        numberOfMovements += abs(head - requestsCopy[i]);
        head = requestsCopy[i];
    }

    return numberOfMovements;
}

// shortest seek time first
int sstf(int *requests, int head)
{
    int numberOfMovements = 0;

    // copy requests into identical array
    int requestsCopy[NUM_OF_REQUESTS];
    for (int i = 0; i < NUM_OF_REQUESTS; i++)
        requestsCopy[i] = requests[i];

    // for each new head, find which value is closest and change the head to that
    for (int i = 0; i < NUM_OF_REQUESTS; i++)
    {
        int closestIndex = -1;
        int shortestLen = UPPER_BOUND + 1;

        // find shortest seek time value in the requests
        for (int j = 0; j < NUM_OF_REQUESTS; j++)
        {
            if (requestsCopy[j] == -1)
            {
                continue;
            }
            int len = abs(head - requestsCopy[j]);
            if (len < shortestLen)
            {
                closestIndex = j;
                shortestLen = len;
            }
        }

        numberOfMovements += shortestLen;
        head = requestsCopy[closestIndex];
        requestsCopy[closestIndex] = -1;
    }

    return numberOfMovements;
}

// start at head, scan leftwards until beginning of disk, then scan rightwards until end of disk
int scan(int *requests, int head)
{
    int numberOfMovements = 0;

    // copy requests into identical array
    int requestsCopy[NUM_OF_REQUESTS];
    for (int i = 0; i < NUM_OF_REQUESTS; i++)
        requestsCopy[i] = requests[i];

    int originalHead = head;

    for (int i = originalHead; i >= 0; i--)
    {
        for (int j = 0; j < NUM_OF_REQUESTS; j++)
        {
            // found the number
            if (requestsCopy[j] == i)
            {
                numberOfMovements += abs(head - i);
                head = i;
                break;
            }
        }
    }

    // SCAN goes back to the beginning of the disk before reversing the order
    numberOfMovements += head;
    head = 0;

    for (int i = originalHead + 1; i < UPPER_BOUND; i++)
    {
        for (int j = 0; j < NUM_OF_REQUESTS; j++)
        {
            // found the number
            if (requestsCopy[j] == i)
            {
                numberOfMovements += abs(head - i);
                head = i;
                break;
            }
        }
    }

    return numberOfMovements;
}

// Scan right-wards until you reach the end of the disk, then jump to the beginning and continue
int cscan(int *requests, int head)
{
    int numberOfMovements = 0;

    int originalHead = head;

    // copy requests into identical array
    int requestsCopy[NUM_OF_REQUESTS];
    for (int i = 0; i < NUM_OF_REQUESTS; i++)
        requestsCopy[i] = requests[i];

    for (int i = originalHead; i < UPPER_BOUND; i++)
    {
        for (int j = 0; j < NUM_OF_REQUESTS; j++)
        {
            // found the number
            if (requestsCopy[j] == i)
            {
                numberOfMovements += abs(head - i);
                head = i;
                break;
            }
        }
    }

    // C-SCAN moves the head to the end of the disk, goes back to the beginning, and then continues normally
    numberOfMovements += UPPER_BOUND - head;
    // I'm not sure if we're meant to count the jump from end to beginning. If so, uncomment this line
    // numberOfMovements += UPPER_BOUND;
    head = 0;

    for (int i = 0; i < originalHead; i++)
    {
        for (int j = 0; j < NUM_OF_REQUESTS; j++)
        {
            // found the number
            if (requestsCopy[j] == i)
            {
                numberOfMovements += abs(head - i);
                head = i;
                break;
            }
        }
    }

    return numberOfMovements;
}

// just like scan, but we don't jump to the beginning of the disk
int look(int *requests, int head)
{
    int numberOfMovements = 0;

    // copy requests into identical array
    int requestsCopy[NUM_OF_REQUESTS];
    for (int i = 0; i < NUM_OF_REQUESTS; i++)
        requestsCopy[i] = requests[i];

    int originalHead = head;

    for (int i = originalHead; i < UPPER_BOUND; i++)
    {
        for (int j = 0; j < NUM_OF_REQUESTS; j++)
        {
            // found the number
            if (requestsCopy[j] == i)
            {
                numberOfMovements += abs(head - i);
                head = i;
                break;
            }
        }
    }

    for (int i = 0; i < originalHead; i++)
    {
        for (int j = 0; j < NUM_OF_REQUESTS; j++)
        {
            // found the number
            if (requestsCopy[j] == i)
            {
                numberOfMovements += abs(head - i);
                head = i;
                break;
            }
        }
    }

    return numberOfMovements;
}

// just like cscan, but we don't jump to the end or to the beginning of the disk.
// instead we jump from the last request to the first one
int clook(int *requests, int head)
{
    int numberOfMovements = 0;

    int originalHead = head;

    // copy requests into identical array
    int requestsCopy[NUM_OF_REQUESTS];
    for (int i = 0; i < NUM_OF_REQUESTS; i++)
        requestsCopy[i] = requests[i];

    for (int i = originalHead; i < UPPER_BOUND; i++)
    {
        for (int j = 0; j < NUM_OF_REQUESTS; j++)
        {
            // found the number
            if (requestsCopy[j] == i)
            {
                numberOfMovements += abs(head - i);
                head = i;
                break;
            }
        }
    }

    for (int i = 0; i < originalHead; i++)
    {
        for (int j = 0; j < NUM_OF_REQUESTS; j++)
        {
            // found the number
            if (requestsCopy[j] == i)
            {
                numberOfMovements += abs(head - i);
                head = i;
                break;
            }
        }
    }

    return numberOfMovements;
}

int main(int argc, char *argv[])
{
    // Get initial position of disk head in args
    if (argc < 2)
    {
        printf("Invalid number of arguments.\n");
        exit(1);
    }
    sscanf(argv[1], "%d", &initialHead);

    int requests[NUM_OF_REQUESTS];

    srand(time(NULL));

    for (int i = 0; i < NUM_OF_REQUESTS; i++)
    {
        int r = (rand() % UPPER_BOUND);
        requests[i] = r;
    }

    printf("FCFS: %d\n", fcfs(requests, initialHead));
    printf("SSTF: %d\n", sstf(requests, initialHead));
    printf("SCAN: %d\n", scan(requests, initialHead));
    printf("C-SCAN: %d\n", cscan(requests, initialHead));
    printf("Look: %d\n", look(requests, initialHead));
    printf("C-Look: %d\n", clook(requests, initialHead));

    return 0;
}