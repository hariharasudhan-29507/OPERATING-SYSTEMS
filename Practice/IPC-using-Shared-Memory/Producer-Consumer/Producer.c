#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE 10

int main()
{
    int shmid, *buf;
    int num;

    shmid = shmget(4777, sizeof(int) * (SIZE + 2), IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    buf = (int *)shmat(shmid, NULL, 0);
    if (buf == (void *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // Initialize indices
    buf[SIZE]   = 0; // in
    buf[SIZE+1] = 0; // out

    printf("Producer: Enter data:):\n");

    while (1)
    {
        printf("Enter data: ");
        scanf("%d", &num);

        // Stop production if -1 is entered
        if (num == -1)
        {
            // Write -1 as sentinel
            while (buf[SIZE] - buf[SIZE+1] >= SIZE)
            {
                usleep(1000); // wait if buffer full
            }
            buf[buf[SIZE] % SIZE] = -1;
            buf[SIZE]++; // increment in
            break;
        }

        // Wait if buffer is full
        while (buf[SIZE] - buf[SIZE+1] >= SIZE)
        {
            usleep(1000);
        }

        // Write to circular buffer
        buf[buf[SIZE] % SIZE] = num;
        buf[SIZE]++; // increment in after write
    }

    shmdt(buf);
    return 0;
}
