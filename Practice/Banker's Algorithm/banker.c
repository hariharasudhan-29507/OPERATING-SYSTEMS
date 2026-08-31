#include <stdio.h>
#include <stdbool.h>

void calculateNeed(int n, int m, int need[n][m], int max[n][m], int allocation[n][m]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - allocation[i][j];
}

void calculateAvailable(int n, int m, int available[m], int total[m], int allocation[n][m]) {
    for (int j = 0; j < m; j++) {
        int sum = 0;
        for (int i = 0; i < n; i++)
            sum += allocation[i][j];
        available[j] = total[j] - sum;
    }
}

bool isSafe(int n, int m, int allocation[n][m], int need[n][m], int available[m], int safeSeq[n]) {
    int work[m];
    bool finish[n];
    for (int i = 0; i < m; i++) work[i] = available[i];
    for (int i = 0; i < n; i++) finish[i] = false;

    int count = 0;
    while (count < n) {
        bool found = false;
        for (int p = 0; p < n; p++) {
            if (!finish[p]) {
                bool can = true;
                for (int j = 0; j < m; j++) {
                    if (need[p][j] > work[j]) { can = false; break; }
                }
                if (can) {
                    for (int j = 0; j < m; j++) work[j] += allocation[p][j];
                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }
        if (!found) return false;
    }
    return true;
}

bool requestResources(int n, int m, int process, int request[m], int allocation[n][m], int need[n][m], int available[m]) {
    for (int i = 0; i < m; i++) {
        if (request[i] > need[process][i]) {
            printf("Error: Request exceeds need.\n");
            return false;
        }
    }
    for (int i = 0; i < m; i++) {
        if (request[i] > available[i]) {
            printf("Process P%d must wait (resources not available).\n", process);
            return false;
        }
    }
    for (int i = 0; i < m; i++) {
        available[i] -= request[i];
        allocation[process][i] += request[i];
        need[process][i] -= request[i];
    }
    int safeSeq[n];
    if (isSafe(n, m, allocation, need, available, safeSeq)) {
        printf("Request granted. System is in SAFE STATE.\nSafe Sequence: ");
        for (int i = 0; i < n; i++) printf("P%d ", safeSeq[i]);
        printf("\n");
        return true;
    } else {
        printf("Request denied. System would enter UNSAFE STATE. Rolling back.\n");
        for (int i = 0; i < m; i++) {
            available[i] += request[i];
            allocation[process][i] -= request[i];
            need[process][i] += request[i];
        }
        return false;
    }
}

int main() {
    int n, m;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resources: ");
    scanf("%d", &m);

    int total[m];
    printf("Enter total instances of each resource in the system:\n");
    for (int i = 0; i < m; i++)
        scanf("%d", &total[i]);

    int allocation[n][m];
    int max[n][m];
    int need[n][m];

    printf("Enter Allocation Matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &allocation[i][j]);

    printf("Enter Max Matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &max[i][j]);

    int available[m];
    int choice;
    printf("\nChoose method to obtain Available matrix:\n");
    printf("1. Enter Available matrix manually\n");
    printf("2. Calculate from Total instances (Available = Total - Sum of Allocated)\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter Available Resources:\n");
        for (int i = 0; i < m; i++)
            scanf("%d", &available[i]);
    } else if (choice == 2) {
        calculateAvailable(n, m, available, total, allocation);
    } else {
        printf("Invalid choice. Exiting.\n");
        return 1;
    }

    calculateNeed(n, m, need, max, allocation);

    printf("\nNeed Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }

    printf("\nAvailable Resources:\n");
    for (int j = 0; j < m; j++)
        printf("%d ", available[j]);
    printf("\n");

    int safeSeq[n];
    if (isSafe(n, m, allocation, need, available, safeSeq)) {
        printf("\nSystem is in SAFE STATE.\nSafe Sequence: ");
        for (int i = 0; i < n; i++) printf("P%d ", safeSeq[i]);
        printf("\n");
    } else {
        printf("\nSystem is in UNSAFE STATE.\n");
    }

    char reqChoice;
    do {
        int process;
        printf("\nEnter process number for resource request: ");
        scanf("%d", &process);

        int request[m];
        printf("Enter request vector:\n");
        for (int i = 0; i < m; i++)
            scanf("%d", &request[i]);

        requestResources(n, m, process, request, allocation, need, available);

        printf("\nDo you want to make another request? (y/n): ");
        scanf(" %c", &reqChoice);
    } while (reqChoice == 'y' || reqChoice == 'Y');

    return 0;
}
