#include <semaphore.h>
#include <stdlib.h>

typedef struct {
    sem_t first_done;
    sem_t second_done;
} Foo;

void printFirst();
void printSecond();
void printThird();

Foo* fooCreate() {
    Foo* obj = (Foo*) malloc(sizeof(Foo));
    sem_init(&obj->first_done, 0, 0);
    sem_init(&obj->second_done, 0, 0);
    return obj;
}

void first(Foo* obj) {
    printFirst();
    sem_post(&obj->first_done);
}

void second(Foo* obj) {
    sem_wait(&obj->first_done);
    printSecond();
    sem_post(&obj->second_done);
}

void third(Foo* obj) {
    sem_wait(&obj->second_done);
    printThird();
}

void fooFree(Foo* obj) {
    sem_destroy(&obj->first_done);
    sem_destroy(&obj->second_done);
    free(obj);
}
