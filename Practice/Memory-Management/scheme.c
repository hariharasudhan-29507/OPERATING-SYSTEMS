#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_BLOCKS 100
#define MAX_PROCESSES 100

typedef struct {
    int id;
    int size;
    int allocated_block;
    int is_allocated;
} Process;

typedef struct {
    int id;
    int size;
    int is_free;
    int remaining;
} MemoryBlock;

int first_fit(MemoryBlock memory[], int block_count, Process *p) {
    for (int i = 0; i < block_count; i++) {
        if (memory[i].is_free && memory[i].size >= p->size) {
            p->allocated_block = i;
            p->is_allocated = 1;
            memory[i].is_free = 0;
            memory[i].remaining = memory[i].size - p->size;
            return i;
        }
    }
    return -1;
}

int best_fit(MemoryBlock memory[], int block_count, Process *p) {
    int best_idx = -1;
    int min_diff = INT_MAX;
    for (int i = 0; i < block_count; i++) {
        if (memory[i].is_free && memory[i].size >= p->size) {
            int diff = memory[i].size - p->size;
            if (diff < min_diff) {
                min_diff = diff;
                best_idx = i;
            }
        }
    }
    if (best_idx != -1) {
        p->allocated_block = best_idx;
        p->is_allocated = 1;
        memory[best_idx].is_free = 0;
        memory[best_idx].remaining = memory[best_idx].size - p->size;
    }
    return best_idx;
}

int worst_fit(MemoryBlock memory[], int block_count, Process *p) {
    int worst_idx = -1;
    int max_diff = -1;
    for (int i = 0; i < block_count; i++) {
        if (memory[i].is_free && memory[i].size >= p->size) {
            int diff = memory[i].size - p->size;
            if (diff > max_diff) {
                max_diff = diff;
                worst_idx = i;
            }
        }
    }
    if (worst_idx != -1) {
        p->allocated_block = worst_idx;
        p->is_allocated = 1;
        memory[worst_idx].is_free = 0;
        memory[worst_idx].remaining = memory[worst_idx].size - p->size;
    }
    return worst_idx;
}

int main() {
    MemoryBlock memory[MAX_BLOCKS];
    Process processes[MAX_PROCESSES];
    MemoryBlock original_memory[MAX_BLOCKS];
    int block_count, process_count;
    int choice;

    printf("Enter number of memory blocks: ");
    scanf("%d", &block_count);
    printf("Enter sizes of %d memory blocks:\n", block_count);
    for (int i = 0; i < block_count; i++) {
        printf("Block %d size: ", i);
        scanf("%d", &memory[i].size);
        memory[i].id = i;
        memory[i].is_free = 1;
        memory[i].remaining = memory[i].size;
        original_memory[i] = memory[i];
    }

    printf("Enter number of processes: ");
    scanf("%d", &process_count);
    printf("Enter sizes of %d processes:\n", process_count);
    for (int i = 0; i < process_count; i++) {
        printf("Process %d size: ", i);
        scanf("%d", &processes[i].size);
        processes[i].id = i;
        processes[i].allocated_block = -1;
        processes[i].is_allocated = 0;
    }

    do {
        printf("\n--- Memory Allocation Menu ---\n");
        printf("1. First Fit\n");
        printf("2. Best Fit\n");
        printf("3. Worst Fit\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice >= 1 && choice <= 3) {
            for (int i = 0; i < block_count; i++) {
                memory[i] = original_memory[i];
            }
            for (int i = 0; i < process_count; i++) {
                processes[i].allocated_block = -1;
                processes[i].is_allocated = 0;
            }

            if (choice == 1) {
                printf("\n=== FIRST FIT ===\n");
                for (int i = 0; i < process_count; i++) {
                    first_fit(memory, block_count, &processes[i]);
                }
            } else if (choice == 2) {
                printf("\n=== BEST FIT ===\n");
                for (int i = 0; i < process_count; i++) {
                    best_fit(memory, block_count, &processes[i]);
                }
            } else if (choice == 3) {
                printf("\n=== WORST FIT ===\n");
                for (int i = 0; i < process_count; i++) {
                    worst_fit(memory, block_count, &processes[i]);
                }
            }

            printf("\nMemory Blocks:\n");
            printf("%-10s %-10s %-10s %-10s\n", "Block ID", "Size", "Free", "Remaining");
            for (int i = 0; i < block_count; i++) {
                printf("%-10d %-10d %-10s %-10d\n", memory[i].id, memory[i].size,
                       memory[i].is_free ? "Yes" : "No", memory[i].remaining);
            }

            printf("\nProcesses:\n");
            printf("%-10s %-10s %-15s %-10s\n", "Process ID", "Size", "Allocated Block", "Status");
            for (int i = 0; i < process_count; i++) {
                printf("%-10d %-10d %-15d %-10s\n", processes[i].id, processes[i].size,
                       processes[i].allocated_block,
                       processes[i].is_allocated ? "Allocated" : "Waiting");
            }
        } else if (choice != 4) {
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    printf("Exiting program.\n");
    return 0;
}
