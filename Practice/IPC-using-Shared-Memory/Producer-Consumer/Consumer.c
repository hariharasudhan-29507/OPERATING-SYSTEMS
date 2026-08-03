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

    shmid = shmget(4777, sizeof(int) * (SIZE + 2), 0666);
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

    printf("Consumer: Reading data :\n");

    int producer_done = 0;

    while (1)
    {
        
        while (buf[SIZE] == buf[SIZE+1] && !producer_done)
        {
            usleep(1000);
        }

        if (buf[SIZE] == buf[SIZE+1] && producer_done)
            break;

        
        num = buf[buf[SIZE+1] % SIZE];

        
        if (num == -1)
        {
            producer_done = 1;
            buf[SIZE+1]++; 
            continue;
        }

        printf("%d\n", num);
        buf[SIZE+1]++; 
    }

    shmdt(buf);
    shmctl(shmid, IPC_RMID, NULL); 

    return 0;
}
