#ifndef PROTO_H__
#define PROTO_H__

#define MGROUP  "224.2.2.2"
#define RCVPORT "2001"

#define NAMESIZE 25
#define NUM_SUBJECTS 4

struct msg_st {
    uint8_t name[NAMESIZE];
    uint16_t score[NUM_SUBJECTS];
}__attribute__((packed));




#endif
