typedef struct {
    pthread_mutex_t mutex;
    sem_t hydroQueue;
    sem_t oxyQueue;
    pthread_barrier_t barrier;
    int hydrogen;
    int oxygen;
} H2O;

H2O* h2oCreate() {
    H2O* obj = (H2O*)malloc(sizeof(H2O));
    pthread_mutex_init(&obj->mutex, NULL);
    sem_init(&obj->hydroQueue, 0, 0);
    sem_init(&obj->oxyQueue, 0, 0);
    pthread_barrier_init(&obj->barrier, NULL, 3);
    obj->hydrogen = 0;
    obj->oxygen = 0;
    return obj;
}

void hydrogen(H2O* obj) {
    int leader = 0;
    pthread_mutex_lock(&obj->mutex);
    obj->hydrogen++;
    if (obj->hydrogen >= 2 && obj->oxygen >= 1) {
        leader = 1;
        sem_post(&obj->hydroQueue);
        obj->hydrogen -= 2;
        sem_post(&obj->oxyQueue);
        obj->oxygen -= 1;
    } else {
        pthread_mutex_unlock(&obj->mutex);
    }
    if (!leader) {
        sem_wait(&obj->hydroQueue);
        releaseHydrogen();
        pthread_barrier_wait(&obj->barrier);
        return;
    }
    releaseHydrogen();
    pthread_barrier_wait(&obj->barrier);
    pthread_mutex_unlock(&obj->mutex);
}

void oxygen(H2O* obj) {
    int leader = 0;
    pthread_mutex_lock(&obj->mutex);
    obj->oxygen++;
    if (obj->hydrogen >= 2) {
        leader = 1;
        sem_post(&obj->hydroQueue);
        sem_post(&obj->hydroQueue);
        obj->hydrogen -= 2;
        obj->oxygen -= 1;
    } else {
        pthread_mutex_unlock(&obj->mutex);
    }
    if (!leader) {
        sem_wait(&obj->oxyQueue);
        releaseOxygen();
        pthread_barrier_wait(&obj->barrier);
        return;
    }
    releaseOxygen();
    pthread_barrier_wait(&obj->barrier);
    pthread_mutex_unlock(&obj->mutex);
}

void h2oFree(H2O* obj) {
    pthread_mutex_destroy(&obj->mutex);
    sem_destroy(&obj->hydroQueue);
    sem_destroy(&obj->oxyQueue);
    pthread_barrier_destroy(&obj->barrier);
    free(obj);
}
