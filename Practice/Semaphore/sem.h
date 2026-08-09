#ifndef SEM_H
#define SEM_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE 5
#define MUTEX 0
#define FULL  1
#define EMPTY 2

#define SHM_KEY 4777
#define SEM_KEY 5777

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void wait_sem(int sid, int sno);
void signal_sem(int sid, int sno);

#endif
