#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128
#define ID 1

typedef struct msgbuf
{
    long mtype;
    int cliente;
    char mtext[MSGSZ];
} message_buf;

int main()
{
    int msqid;
    key_t key = 5678;
    message_buf sbuf, rbuf;
    size_t buf_length = sizeof(message_buf) - sizeof(long);

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0)
    {
        perror("msgget");
        exit(1);
    }

    sbuf.mtype = 1;
    sbuf.cliente = ID;
    sprintf(sbuf.mtext, "Hola servidor, soy el cliente %d", ID);

    if (msgsnd(msqid, &sbuf, buf_length, 0) < 0)
    {
        perror("msgsnd");
        exit(1);
    }
    printf("[Cliente %d] Envio: %s\n", ID, sbuf.mtext);

    if (msgrcv(msqid, &rbuf, buf_length, ID + 1, 0) < 0)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("[Cliente %d] Recibio: %s\n", ID, rbuf.mtext);

    exit(0);
}