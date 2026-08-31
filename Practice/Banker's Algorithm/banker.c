#include <stdio.h>
#include <stdbool.h>

#define P 5
#define R 3

int available[R];
int max[P][R];
int allocation[P][R];
int need[P][R];

void calculateNeed() {
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

bool isSafe(int safeSeq[]) {
    int work[R];
    bool finish[P] = {false};

    for (int i = 0; i < R; i++) {
        work[i] = available[i];
    }

    int count = 0;
    while (count < P) {
        bool found = false;
        for (int p = 0; p < P; p++) {
            if (!finish[p]) {
                bool canAllocate = true;
                for (int j = 0; j < R; j++) {
                    if (need[p][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                if (canAllocate) {
                    for (int j = 0; j < R; j++) {
                        work[j] += allocation[p][j];
                    }
                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            return false;
        }
    }
    return true;
}

bool requestResources(int process, int request[]) {
    printf("\n--- Process P%d requesting resources ---\n", process);
    printf("Request: ");
    for (int i = 0; i < R; i++) printf("%d ", request[i]);
    printf("\n");

    for (int i = 0; i < R; i++) {
        if (request[i] > need[process][i]) {
            printf("Error: Request exceeds maximum claim (need).\n");
            return false;
        }
    }

    for (int i = 0; i < R; i++) {
        if (request[i] > available[i]) {
            printf("Process P%d must wait (resources not available).\n", process);
            return false;
        }
    }

    printf("Pretending to allocate resources...\n");
    for (int i = 0; i < R; i++) {
        available[i] -= request[i];
        allocation[process][i] += request[i];
        need[process][i] -= request[i];
    }

    int safeSeq[P];
    if (isSafe(safeSeq)) {
        printf("Request granted. System is in SAFE STATE.\n");
        printf("Safe Sequence: ");
        for (int i = 0; i < P; i++) {
            printf("P%d ", safeSeq[i]);
        }
        printf("\n");
        return true;
    } else {
        printf("Request denied. System would enter UNSAFE STATE. Rolling back.\n");
        for (int i = 0; i < R; i++) {
            available[i] += request[i];
            allocation[process][i] -= request[i];
            need[process][i] += request[i];
        }
        return false;
    }
}

void printState() {
    printf("\n========== CURRENT STATE ==========\n");

    printf("\nAllocation Matrix:\n");
    printf("    A  B  C\n");
    for (int i = 0; i < P; i++) {
        printf("P%d  ", i);
        for (int j = 0; j < R; j++) {
            printf("%d  ", allocation[i][j]);
        }
        printf("\n");
    }

    printf("\nMax Matrix:\n");
    printf("    A  B  C\n");
    for (int i = 0; i < P; i++) {
        printf("P%d  ", i);
        for (int j = 0; j < R; j++) {
            printf("%d  ", max[i][j]);
        }
        printf("\n");
    }

    printf("\nNeed Matrix:\n");
    printf("    A  B  C\n");
    for (int i = 0; i < P; i++) {
        printf("P%d  ", i);
        for (int j = 0; j < R; j++) {
            printf("%d  ", need[i][j]);
        }
        printf("\n");
    }

    printf("\nAvailable: ");
    for (int i = 0; i < R; i++) {
        printf("%d ", available[i]);
    }
    printf("\n");
}

int main() {
    int initAllocation[P][R] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int initMax[P][R] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    int initAvailable[R] = {3, 3, 2};

    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            allocation[i][j] = initAllocation[i][j];
            max[i][j] = initMax[i][j];
        }
    }
    for (int i = 0; i < R; i++) {
        available[i] = initAvailable[i];
    }

    calculateNeed();
    printState();

    int safeSeq[P];
    printf("\n========== INITIAL SAFETY CHECK ==========\n");
    if (isSafe(safeSeq)) {
        printf("System is in SAFE STATE.\n");
        printf("Safe Sequence: ");
        for (int i = 0; i < P; i++) {
            printf("P%d ", safeSeq[i]);
        }
        printf("\n");
    } else {
        printf("System is in UNSAFE STATE.\n");
    }

    int req1[R] = {1, 0, 2};
    requestResources(1, req1);
    printState();

    int req2[R] = {3, 3, 3};
    requestResources(4, req2);

    int req3[R] = {0, 2, 0};
    requestResources(0, req3);
    printState();

    int req4[R] = {0, 0, 2};
    requestResources(3, req4);
    printState();

    return 0;
}
