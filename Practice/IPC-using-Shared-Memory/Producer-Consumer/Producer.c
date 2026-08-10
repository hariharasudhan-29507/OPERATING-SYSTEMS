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
    // Use volatile to prevent the compiler from optimizing out memory reads/writes
    volatile int *buf;
    int num;

    // Create the shared memory segment
    shmid = shmget(SHM_KEY, sizeof(int) * (SIZE + 2), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    buf = (volatile int *)shmat(shmid, NULL, 0);
    if (buf == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // Initialize the shared state indices ONLY once here
    buf[SIZE]   = 0; // in
    buf[SIZE+1] = 0; // out

    printf("Producer Active. Enter integers (-1 to stop):\n");

    while (1) {
        printf("Enter data: ");
        if (scanf("%d", &num) != 1) break;

        // BLOCK: Wait if the circular queue is fully saturated
        while ((buf[SIZE] - buf[SIZE+1]) >= SIZE) {
            usleep(100000); // 100ms pause to save CPU cycles
        }

        // Write element into safe circular index
        buf[buf[SIZE] % SIZE] = num;
        
        // Use a memory fence equivalent by incrementing right after data commitment
        buf[SIZE]++; 

        if (num == -1) {
            break;
        }
    }

    shmdt((void *)buf);
    printf("Producer Finished.\n");
    return 0;
}
