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
    struct msg_st *p_rcv_buf; 
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len = sizeof(remote_addr);
    char ipstr[IPSTRSIZE];

    p_rcv_buf = msg_st_create(NAMEMAX);
    if (p_rcv_buf == NULL) 
        exit(1);

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("socket()");
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
        recvfrom(sd, p_rcv_buf, msg_st_size(p_rcv_buf), 0,
                 (void *)&remote_addr, &remote_addr_len);
        inet_ntop(AF_INET, &remote_addr.sin_addr, ipstr, IPSTRSIZE);
        printf("---MESSAGE FROM %s:%d---\n", 
                ipstr, 
                ntohs(remote_addr.sin_port));
        printf("NAME = %s\n", p_rcv_buf->name);
        for (i = 0; i < NUM_SUBJECTS; i++) {
            printf("SCORE OF SUBJECT#%d: %hd\n", 
                    i + 1, 
                    ntohs(p_rcv_buf->score[i]));
        }

        msg_st_init(p_rcv_buf, NAMEMAX);
    }

    close();

    exit(0);
}
