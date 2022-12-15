#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NUM_OF_REQUESTS 1000
#define DISK_SIZE 5000

int disk[DISK_SIZE];
int initialHead;

void insertionSort(int *array)
{
    int i, key, j;
    for (i = 1; i < NUM_OF_REQUESTS; i++)
    {
        key = array[i];
        j = i - 1;

        while (j >= 0 && array[j] > key)
        {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }
}

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
        int shortestLen = DISK_SIZE + 1;

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
    {
        requestsCopy[i] = requests[i];
    }

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

    for (int i = originalHead + 1; i < DISK_SIZE; i++)
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

    for (int i = originalHead; i < DISK_SIZE; i++)
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
    numberOfMovements += (DISK_SIZE - 1) - head;
    // I'm not sure if we're meant to count the jump from end to beginning. If so, uncomment this line
    // numberOfMovements += DISK_SIZE;
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
    {
        requestsCopy[i] = requests[i];
    }

    insertionSort(requestsCopy);

    int k = 0;
    int originalHead = head;

    // Find index to start at
    while (originalHead > requestsCopy[k])
    {
        k++;
    }

    for (int i = k; i < NUM_OF_REQUESTS; i++)
    {
        numberOfMovements += abs(head - requestsCopy[i]);
        head = requestsCopy[i];
    }

    for (int i = k - 1; i >= 0; i--)
    {
        numberOfMovements += abs(head - requestsCopy[i]);
        head = requestsCopy[i];
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
    {
        requestsCopy[i] = requests[i];
    }

    insertionSort(requestsCopy);

    int k = 0;

    // Find index to start at
    while (originalHead > requestsCopy[k])
    {
        k++;
    }

    for (int i = k; i < NUM_OF_REQUESTS; i++)
    {
        numberOfMovements += abs(head - requestsCopy[i]);
        head = requestsCopy[i];
    }

    for (int i = 0; i <= k; i++)
    {
        numberOfMovements += abs(head - requestsCopy[i]);
        head = requestsCopy[i];
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
        int r = (rand() % DISK_SIZE);
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