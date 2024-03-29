#include "lib/hiredis/hiredis.h"

#ifndef REDIS_H
#define REDIS_H

typedef enum { 
    BLOCK, 
    PEER
} KEY_TYPE;

typedef struct redisConf {
    const char ip[15];
    const int port;
    const char password[512];
    const struct timeval connection_timeout;
} redisConfiguration;

typedef struct redisCli {
    redisConfiguration* configuration;
    redisContext* context;
} redisClient;

char** get_all_peers_command(int* resultAmount);
void set_redis_command(KEY_TYPE type, char* key, char* value);
void get_redis_command(KEY_TYPE type, char* key, char* result);
void del_redis_command(KEY_TYPE type, char* key);

void changeDatabase(KEY_TYPE type);
int init_redis();
void dispose();
void clean_registry(KEY_TYPE type);

#endif //REDIS_H