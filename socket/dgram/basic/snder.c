#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "proto.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage...");
        exit(1);
    }
    
    int sd, i;
    struct msg_st send_buf;
    struct sockaddr_in remote_addr;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }

    memset(&send_buf, '\0', sizeof(send_buf));
    strcpy(send_buf.name, "Chtholly");
    for (i = 0; i < NUM_SUBJECTS; i++)
        send_buf.score[i] = htons(rand() % 100);

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET, argv[1], &remote_addr.sin_addr);

    if (sendto(sd, &send_buf, sizeof(send_buf), 0, 
        (void *)&remote_addr, sizeof(remote_addr)) < 0) {
        perror("sendto()");
        exit(1);
    }

    puts("OK!");

    close();

    exit(0);
}
