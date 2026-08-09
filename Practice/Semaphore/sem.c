#include "sem.h"

void wait_sem(int sid, int sno) {
    struct sembuf s;
    s.sem_num = sno;
    s.sem_op = -1;
    s.sem_flg = 0;
    if (semop(sid, &s, 1) == -1)
        perror("semop wait");
}

void signal_sem(int sid, int sno) {
    struct sembuf s;
    s.sem_num = sno;
    s.sem_op = 1;
    s.sem_flg = 0;
    if (semop(sid, &s, 1) == -1)
        perror("semop signal");
}
