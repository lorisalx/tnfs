#include "hiredis/hiredis.h"

#ifndef REDIS_H
#define REDIS_H

struct {
    const char ip[15];
    const int port;
    const char password[512];
    const struct timeval connection_timeout;
} typedef redisConfiguration;

struct {
    redisConfiguration* configuration;
    redisContext* context;
} typedef redisClient;

void set_redis_configuration(redisConfiguration* new_configuration);
void set_redis_command(char* key, char* value);
char* get_redis_command(char* key);
void dispose();

#endif //REDIS_H