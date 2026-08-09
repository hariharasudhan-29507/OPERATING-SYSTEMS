#include "sem.h"

int main() {
    int sid, s, out = 0, num, count;
    int *buf;

    s = shmget(SHM_KEY, sizeof(int) * SIZE, 0600);
    if (s == -1) { perror("shmget"); exit(1); }

    buf = (int *) shmat(s, NULL, 0);
    if (buf == (void *) -1) { perror("shmat"); exit(1); }

    sid = semget(SEM_KEY, 3, 0600);
    if (sid == -1) { perror("semget"); exit(1); }

    printf("How many items to consume? ");
    scanf("%d", &count);

    printf("Data Read:\n");
    for (int i = 0; i < count; i++) {
        wait_sem(sid, FULL);
        wait_sem(sid, MUTEX);

        num = buf[out % SIZE];
        printf("Consumed: %d from index %d\n", num, out % SIZE);
        out++;

        signal_sem(sid, MUTEX);
        signal_sem(sid, EMPTY);
    }

    shmdt(buf);
    semctl(sid, 0, IPC_RMID);
    shmctl(s, IPC_RMID, NULL);
    return 0;
}
