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

MemoryBlock memory[MAX_BLOCKS];
Process processes[MAX_PROCESSES];
int block_count = 0;
int process_count = 0;

void initialize_memory(int blocks[], int n) {
    block_count = n;
    for (int i = 0; i < n; i++) {
        memory[i].id = i;
        memory[i].size = blocks[i];
        memory[i].is_free = 1;
        memory[i].remaining = blocks[i];
    }
}

void reset_memory(int blocks[], int n) {
    for (int i = 0; i < n; i++) {
        memory[i].is_free = 1;
        memory[i].remaining = memory[i].size;
    }
    for (int i = 0; i < process_count; i++) {
        processes[i].allocated_block = -1;
        processes[i].is_allocated = 0;
    }
}

void add_processes(int sizes[], int n) {
    process_count = n;
    for (int i = 0; i < n; i++) {
        processes[i].id = i;
        processes[i].size = sizes[i];
        processes[i].allocated_block = -1;
        processes[i].is_allocated = 0;
    }
}

int first_fit(Process *p) {
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

int best_fit(Process *p) {
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

int worst_fit(Process *p) {
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

void allocate_all(int (*fit_func)(Process *)) {
    for (int i = 0; i < process_count; i++) {
        if (!processes[i].is_allocated) {
            fit_func(&processes[i]);
        }
    }
}

void deallocate_process(int process_id) {
    for (int i = 0; i < process_count; i++) {
        if (processes[i].id == process_id && processes[i].is_allocated) {
            int block_idx = processes[i].allocated_block;
            memory[block_idx].is_free = 1;
            memory[block_idx].remaining = memory[block_idx].size;
            processes[i].allocated_block = -1;
            processes[i].is_allocated = 0;
            return;
        }
    }
}

void print_memory_status() {
    printf("\nMemory Blocks:\n");
    printf("%-10s %-10s %-10s %-10s\n", "Block ID", "Size", "Free", "Remaining");
    for (int i = 0; i < block_count; i++) {
        printf("%-10d %-10d %-10s %-10d\n", memory[i].id, memory[i].size,
               memory[i].is_free ? "Yes" : "No", memory[i].remaining);
    }
}

void print_process_status() {
    printf("\nProcesses:\n");
    printf("%-10s %-10s %-15s %-10s\n", "Process ID", "Size", "Allocated Block", "Status");
    for (int i = 0; i < process_count; i++) {
        printf("%-10d %-10d %-15d %-10s\n", processes[i].id, processes[i].size,
               processes[i].allocated_block,
               processes[i].is_allocated ? "Allocated" : "Waiting");
    }
}

void run_first_fit(int mem_blocks[], int m, int proc_sizes[], int p) {
    printf("=== FIRST FIT ===\n");
    initialize_memory(mem_blocks, m);
    add_processes(proc_sizes, p);
    allocate_all(first_fit);
    print_memory_status();
    print_process_status();
}

void run_best_fit(int mem_blocks[], int m, int proc_sizes[], int p) {
    printf("\n=== BEST FIT ===\n");
    reset_memory(mem_blocks, m);
    add_processes(proc_sizes, p);
    allocate_all(best_fit);
    print_memory_status();
    print_process_status();
}

void run_worst_fit(int mem_blocks[], int m, int proc_sizes[], int p) {
    printf("\n=== WORST FIT ===\n");
    reset_memory(mem_blocks, m);
    add_processes(proc_sizes, p);
    allocate_all(worst_fit);
    print_memory_status();
    print_process_status();
}

int main() {
    int mem_blocks[] = {100, 500, 200, 300, 600};
    int proc_sizes[] = {212, 417, 112, 426};
    int m = sizeof(mem_blocks) / sizeof(mem_blocks[0]);
    int p = sizeof(proc_sizes) / sizeof(proc_sizes[0]);

    run_first_fit(mem_blocks, m, proc_sizes, p);
    run_best_fit(mem_blocks, m, proc_sizes, p);
    run_worst_fit(mem_blocks, m, proc_sizes, p);

    return 0;
}
