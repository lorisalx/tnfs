#include "redis.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>

static redisClient client;

void init_redis()
{
    redisConfiguration default_conf = {"localhost", 6379, "", {1, 50000}};
    client.configuration = &default_conf;

    redisContext *c = redisConnectWithTimeout(client.configuration->ip, client.configuration->port, client.configuration->connection_timeout);
    if (c != NULL && c->err) {
        log_error(c->errstr);
    }
    client.context = c;
    log_info("New redis context successfully instantiated.");
}

void dispose()
{
    redisFree(client.context);
}

void set_redis_command(char* key, char* value)
{
    log_formated_info( "New redis SET command called for tuple (%s, %s).", key, value);
    redisReply *reply = redisCommand(client.context,"SET %s %s", key, value);
    freeReplyObject(reply);
}

void del_redis_command(char* key)
{
    log_formated_info( "New redis DEL command called for key (%s).", key);
    redisReply *reply = redisCommand(client.context,"DEL %s", key);
    freeReplyObject(reply);
}

void get_redis_command(char* key, char* result)
{
    redisReply *reply = redisCommand(client.context,"GET %s", key);
    strcpy(result, (reply->str == NULL) ? "null" : reply->str);
    freeReplyObject(reply);
}

void clean_registry() {
    redisCommand(client.context,"FLUSHALL");
}