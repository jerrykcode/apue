#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "proto.h"

#define IPSTRSIZE 40

int main() {

    int sd, i;
    struct sockaddr_in local_addr;
    struct msg_st rcv_buf; 
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len = sizeof(remote_addr);
    char ipstr[IPSTRSIZE];

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }
    
    int val = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val)) < 0) {
        perror("setsockopt()");
        exit(1);
    }

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET, "0.0.0.0", &local_addr.sin_addr);

    if (bind(sd, (void *)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind()");
        exit(1);
    }

    while (1) {
        recvfrom(sd, &rcv_buf, sizeof(rcv_buf), 0,
                 (void *)&remote_addr, &remote_addr_len);
        inet_ntop(AF_INET, &remote_addr.sin_addr, ipstr, IPSTRSIZE);
        printf("---MESSAGE FROM %s:%d---\n", 
                ipstr, 
                ntohs(remote_addr.sin_port));
        printf("NAME = %s\n", rcv_buf.name);
        for (i = 0; i < NUM_SUBJECTS; i++) {
            printf("SCORE OF SUBJECT#%d: %hd\n", 
                    i + 1, 
                    ntohs(rcv_buf.score[i]));
        }
    }

    close();

    exit(0);
}
