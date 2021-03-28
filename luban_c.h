#ifndef LUBAN_LUBAN_C_H
#define LUBAN_LUBAN_C_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PServer pserver;
// create psserver
pserver *damo_init(char *config_file);

// pull parameters
void *damo_pull(pserver *server, void *keys, int len);

// push parameters
void damo_push(pserver *server, void *keys, void *grads, int len, long long global_step);

// release psserver
void damo_release(pserver *server);


#ifdef __cplusplus
} /* end extern "C"*/
#endif

#endif //LUBAN_LUBAN_C_H
