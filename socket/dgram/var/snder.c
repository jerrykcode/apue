#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "proto.h"

int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Usage...");
        exit(1);
    }

    if (strlen(argv[2]) > NAMEMAX) {
        fprintf(stderr, "Usage...");
        exit(1);
    }
    
    int sd, i;
    struct msg_st *p_send_buf;
    struct sockaddr_in remote_addr;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }

    p_send_buf = msg_st_create(strlen(argv[2]));
    if (p_send_buf == NULL)
        exit(1);
    strcpy(p_send_buf->name, argv[2]);

    for (i = 0; i < NUM_SUBJECTS; i++)
        p_send_buf->score[i] = htons(rand() % 100);

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET, argv[1], &remote_addr.sin_addr);

    if (sendto(sd, p_send_buf, msg_st_size(p_send_buf), 0, 
        (void *)&remote_addr, sizeof(remote_addr)) < 0) {
        perror("sendto()");
        exit(1);
    }

    puts("OK!");

    close();

    exit(0);
}
