#include "hiredis/hiredis.h"

#ifndef REDIS_H
#define REDIS_H

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

void set_redis_command(char* key, char* value);
void get_redis_command(char* key, char* result);
void del_redis_command(char* key);
void init_redis();
void dispose();
void clean_registry();

#endif //REDIS_H