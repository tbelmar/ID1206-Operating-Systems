#include <stdio.h>
#include <string.h>
#define FRAMES 256
#define SIZE 256

int memory[FRAMES * SIZE];
int takenFrames[256];

int pageTable[256];
FILE *fileBin;

struct addressComponents
{
    int logicalAddress;
    int pageNumber;
    int offset;
    int physicalAddress;
};

// Loads a page from backup memory into the main memory
int loadPage(int pageNumber)
{
    signed char buffer[256];
    fseek(fileBin, (pageNumber * 256), SEEK_SET);
    for (int i = 0; i < 256; i++)
    {
        if (takenFrames[i] == 0)
        {
            // i is free Frame
            fread(buffer, 1, 256, fileBin);

            // Literally copying over the 256 bytes (one page) from the bin into the memory
            for (int j = 0; j < 256; j++)
            {
                memory[(i * 256) + j] = buffer[j];
            }

            // This frame is now taken
            takenFrames[i] = 1;

            return i;
        }
    }
}

int search(struct addressComponents address)
{
    // TLB
    if (0)
    {
    }
    // PageTable
    else if (pageTable[address.pageNumber] != -1)
    {
        return pageTable[address.pageNumber];
    }
    // PageFault -- The page is not found int he main memory and must search the second backup memory (BACKING_STORE.bin)
    else
    {
        int frame = loadPage(address.pageNumber);
        pageTable[address.pageNumber] = frame;
        address.physicalAddress = (frame * 256) + address.offset;
        return frame;
    }

    return 0;
}

struct addressComponents parseAddress(int logicalAddress)
{

    int pageNumber, offset, addr;
    struct addressComponents address;

    address.logicalAddress = logicalAddress;
    address.offset = logicalAddress & 0xff;
    address.pageNumber = (logicalAddress >> 8) & 0xff;
    // printf("PageN: %d  offset: %d\n", address.pageNumber, address.offset);

    return address;
}

int main()
{
    unsigned char buffer[100];
    struct addressComponents addressList[1000];

    int logicalAddress;
    FILE *addresses;
    addresses = fopen("addresses.txt", "r");
    fileBin = fopen("BACKING_STORE.bin", "rb");

    for (int i = 0; i < 256; i++)
    {
        pageTable[i] = -1;
    }

    for (int i = 0; i < 1000; i++)
    {
        fgets(buffer, 8, addresses);
        sscanf(buffer, "%d", &logicalAddress);
        addressList[i] = parseAddress(logicalAddress);
        addressList[i].physicalAddress = (search(addressList[i]) * 256) + addressList[i].offset;
        printf("logical address: %d", addressList[i].logicalAddress);
        printf("  physical address: %d", addressList[i].physicalAddress);
        signed int lol = memory[addressList[i].physicalAddress];
        printf("  value: %d\n", lol); // Make print signed plz
    }

    return 0;
}