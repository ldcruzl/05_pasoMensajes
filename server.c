#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128
#define NCLIENTES 2

typedef struct msgbuf
{
    long mtype;
    int cliente;
    char mtext[MSGSZ];
} message_buf;

int main()
{
    int msqid, i;
    key_t key = 5678;
    message_buf rbuf, sbuf;
    size_t buf_length = sizeof(message_buf) - sizeof(long);

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0)
    {
        perror("msgget");
        exit(1);
    }
    printf("[Servidor] Esperando clientes...\n");

    for (i = 0; i < NCLIENTES; i++)
    {
        if (msgrcv(msqid, &rbuf, buf_length, 1, 0) < 0)
        {
            perror("msgrcv");
            exit(1);
        }
        printf("[Servidor] Recibio de cliente %d: %s\n", rbuf.cliente, rbuf.mtext);

        sbuf.mtype = rbuf.cliente + 1;
        sbuf.cliente = 0;
        sprintf(sbuf.mtext, "Respuesta privada para el cliente %d", rbuf.cliente);

        if (msgsnd(msqid, &sbuf, buf_length, 0) < 0)
        {
            perror("msgsnd");
            exit(1);
        }
        printf("[Servidor] Respondio al cliente %d\n", rbuf.cliente);
    }

    exit(0);
}