[24bcs040@mepcolinux ex3b]$cat q1.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>


struct msg_buffer {
    long msg_type;      
    int data;       
};

int main() {
    key_t key;
    int msqid,i;
    pid_t pid;

    key = ftok("progfile", 65);
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    
    msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget failed");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        
        struct msg_buffer message;
        
        for (i = 1; i <= 5; i++) {
            
            if (msgrcv(msqid, &message, sizeof(message.data), 1, 0) == -1) {
                perror("msgrcv failed");
                exit(1);
            }
            printf("[Consumer] Consumed item: %d\n", message.data);
            sleep(2);
        }
        exit(0);
    } 
    else {
        
	struct msg_buffer message;
        message.msg_type = 1;

        for (i = 1; i <= 5; i++) {
            message.data = i * 10; 
            printf("[Producer] Produced item: %d\n", message.data);

            
            if (msgsnd(msqid, &message, sizeof(message.data), 0) == -1) {
                perror("msgsnd failed");
                exit(1);
            }
            sleep(1); 
        }

        
        wait(NULL);

        
        msgctl(msqid, IPC_RMID, NULL);
        printf("Message queue destroyed. Program finished.\n");
    }

    return 0;
}
