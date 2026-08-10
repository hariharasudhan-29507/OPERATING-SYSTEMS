#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE 10
#define SHM_KEY 4777

int main() {
    int shmid;
    volatile int *buf;
    int num;

    // Connect to the shared memory mapping allocation
    shmid = shmget(SHM_KEY, sizeof(int) * (SIZE + 2), 0666);
    if (shmid == -1) {
        perror("shmget failed. Please launch the Producer first");
        exit(1);
    }

    buf = (volatile int *)shmat(shmid, NULL, 0);
    if (buf == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    printf("Consumer Active. Waiting for real-time production stream...\n");

    while (1) {
        // BLOCK: Hard wait loop if the write pointer catches up with read pointer
        while (buf[SIZE] == buf[SIZE+1]) {
            usleep(50000); // 50ms check cycle
        }

        // Pick out the current pending index value
        num = buf[buf[SIZE+1] % SIZE];
        
        // Push the tracking pointer forward immediately
        buf[SIZE+1]++; 

        // Check for sentinel exit protocol
        if (num == -1) {
            printf("[Termination Signal Received -1. Closing Pipeline]\n");
            break;
        }

        printf("Consumed: %d\n", num);
    }

    shmdt((void *)buf);
    shmctl(shmid, IPC_RMID, NULL); // Safe system clean up
    return 0;
}
