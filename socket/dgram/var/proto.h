#ifndef PROTO_H__
#define PROTO_H__

#include <stdint.h>
#include <string.h>

#define RCVPORT "2001"

#define NUM_SUBJECTS 4
#define NAMEMAX (512 - NUM_SUBJECTS*2 - 1 - 8)

struct msg_st {
    uint16_t score[NUM_SUBJECTS];
    uint8_t name[1];
}__attribute__((packed));

struct msg_st *msg_st_create(size_t name_size) {
    if (name_size > NAMEMAX)
        return NULL;
    size_t size = sizeof(struct msg_st) + name_size;
    struct msg_st *p_msg = malloc(size);
    if (p_msg == NULL) {
        perror("malloc");
        return NULL;
    }
    memset(p_msg->name, '0', name_size);
    p_msg->name[name_size] = '\0';
    return p_msg;
}

void msg_st_init(struct msg_st *p_msg, size_t name_size) {
    if (p_msg == NULL || name_size > NAMEMAX)
        return;
    memset(p_msg->name, '0', name_size);
    p_msg->name[name_size] = '\0';
}

size_t msg_st_size(struct msg_st *p_msg) {
    if (p_msg == NULL)
        return 0;
    return sizeof(struct msg_st) + strlen(p_msg->name);
}

#endif
