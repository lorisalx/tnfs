#include "redis.h"
#include "logger.h"
#include <stdlib.h>

static redisClient* client;
static redisConfiguration* default_configuration = {"localhost", 6379, "", {1, 50000}};

redisContext* get_redis_context()
{
    if(!client->configuration)
    {
        log_warn("Redis configuration not defined, setting it to default.");
        client->configuration = default_configuration;
    }

    if(!client->context)
    {
        log_warn("Redis context not defined, instantiating a new one.");
        redisContext *c = redisConnectWithTimeout(client->configuration->ip, client->configuration->port, client->configuration->connection_timeout);
        if (c != NULL && c->err) {
            log_error(c->errstr);
            return NULL;
        }
        client->context = c;
        redisFree(c);
        log_info("New redis context successfully instantiated.");
        redisReply *r = redisCommand(c, "AUTH %s", client->configuration->password);
        freeReplyObject(r);
    }
    log_info("Retrieving redis context...");
    return client->context;
}

void set_redis_configuration(redisConfiguration* new_configuration)
{
    client->configuration = new_configuration;
}

void dispose()
{
    redisFree(client->context);
    free(client->configuration);
    free(client);
    free(default_configuration);
}

void set_redis_command(char* key, char* value)
{
    log_formated_info( "New redis set command called for tuple (%s, %s).", key, value);
    redisReply *reply = redisCommand(client->context,"SET %s %s", key, value);
    freeReplyObject(reply);
}

char* get_redis_command(char* key)
{
    log_formated_info("New redis get command called for key (%s, ?).", key);
    redisReply *reply = redisCommand(client->context,"GET %s", key);
    char* res = reply->str;
    freeReplyObject(reply);
    return res;
}