#include <stdio.h>

int main()
{
    int logicalAddress, pageSize, page, offset;
    int n, i;

    printf("Enter logical address: ");
    scanf("%d", &logicalAddress);

    printf("Enter page size: ");
    scanf("%d", &pageSize);

    printf("Enter number of pages: ");
    scanf("%d", &n);

    int pageTable[n];

    printf("Enter page table:\n");
    for (i = 0; i < n; i++)
    {
        printf("Page %d -> Frame ", i);
        scanf("%d", &pageTable[i]);
    }

    page = logicalAddress / pageSize;
    offset = logicalAddress % pageSize;

    if (page >= n)
    {
        printf("Invalid logical address\n");
    }
    else
    {
        int frame = pageTable[page];
        int physicalAddress = frame * pageSize + offset;

        printf("Page Number: %d\n", page);
        printf("Offset: %d\n", offset);
        printf("Frame Number: %d\n", frame);
        printf("Physical Address: %d\n", physicalAddress);
    }

    return 0;
}
