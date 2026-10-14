#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128


typedef struct msgbuf
{
    long mtype;
    char mtext[MSGSZ]; // Un mensaje de longitud MSGSZ
} message_buf;

int main()
{
    int msqid;
    key_t key;
    message_buf rbuf, sbuf;
    size_t buf_length;

    key = 1234;

    if ((msqid = msgget(key, 0666)) < 0)
    {
        perror("msgget");
        exit(1);
    }

    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("[Proceso 2] Recibio: %s\n", rbuf.mtext);

    sbuf.mtype = 2;
    (void)strcpy(sbuf.mtext, "Loud and clear");
    buf_length = strlen(sbuf.mtext) + 1;

    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0)
    {
        perror("msgsnd");
        exit(1);
    }
    printf("[Proceso 2] Envio: %s\n", sbuf.mtext);

    if (msgrcv(msqid, &rbuf, MSGSZ, 3, 0) < 0)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("[Proceso 2] Recibio: %s\n", rbuf.mtext);

    msgctl(msqid, IPC_RMID, NULL);
    exit(0);
}

