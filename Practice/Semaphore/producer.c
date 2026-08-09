#include "sem.h"

int main() {
    int sid, s, in = 0, num, count;
    int *buf;
    union semun snum;
    unsigned short a[3];

    s = shmget(SHM_KEY, sizeof(int) * SIZE, IPC_CREAT | 0600);
    if (s == -1) { perror("shmget"); exit(1); }

    buf = (int *) shmat(s, NULL, 0);
    if (buf == (void *) -1) { perror("shmat"); exit(1); }

    sid = semget(SEM_KEY, 3, IPC_CREAT | 0600);
    if (sid == -1) { perror("semget"); exit(1); }

    a[MUTEX] = 1;
    a[FULL]  = 0;
    a[EMPTY] = SIZE;
    snum.array = a;
    semctl(sid, 0, SETALL, snum);

    printf("How many items to produce? ");
    scanf("%d", &count);

    for (int i = 0; i < count; i++) {
        printf("Enter data item %d: ", i + 1);
        scanf("%d", &num);

        wait_sem(sid, EMPTY);
        wait_sem(sid, MUTEX);

        buf[in % SIZE] = num;
        printf("Produced: %d at index %d\n", num, in % SIZE);
        in++;

        signal_sem(sid, MUTEX);
        signal_sem(sid, FULL);
    }

    shmdt(buf);
    return 0;
}
