#include "redis.h"
#include "logger.h"
#include "peer.h"
#include <stdlib.h>
#include <string.h>

static redisClient client;

int init_redis()
{
    redisConfiguration default_conf = {"localhost", 6379, "", {1, 50000}};
    client.configuration = &default_conf;

    redisContext *c = redisConnectWithTimeout(client.configuration->ip, client.configuration->port, client.configuration->connection_timeout);
    if (c != NULL && c->err) {
        log_error(c->errstr);
        return -1;
    }
    client.context = c;
    //log_info("New redis context successfully instantiated.");
    return 1;
}

void dispose()
{
    redisFree(client.context);
}

char** get_all_peers_command(int* resultAmount)
{
    //log_formated_info( "New redis GET command called for all tuples.");
    changeDatabase(PEER);
    redisReply *reply = redisCommand(client.context,"KEYS *");

    // Create response arrays
    char **peerTab = calloc(reply->elements, sizeof(char *));
    if(reply->elements != 0) {
        for(int i = 0; i < reply->elements; i++) {
            peerTab[i] = calloc(1, sizeof(char) * MAX_PEER_SIZE);
            strcpy(peerTab[i], reply->element[i]->str);
        }
    }

    *resultAmount = reply->elements;
    freeReplyObject(reply);
    return peerTab;
}

void set_redis_command(KEY_TYPE type, char* key, char* value)
{
    //log_formated_info( "New redis SET command called for tuple (%s, %s).", key, value);
    changeDatabase(type);
    redisReply *reply = redisCommand(client.context,"SET %s %s", key, value);
    freeReplyObject(reply);
}

void del_redis_command(KEY_TYPE type, char* key)
{
    //log_formated_info( "New redis DEL command called for key (%s).", key);
    changeDatabase(type);
    redisReply *reply = redisCommand(client.context,"DEL %s", key);
    freeReplyObject(reply);
}

void get_redis_command(KEY_TYPE type, char* key, char* result)
{
    //log_formated_info( "New redis GET command called for key (%s).", key);
    changeDatabase(type);
    redisReply *reply = redisCommand(client.context,"GET %s", key);
    strcpy(result, (reply->str == NULL) ? "null" : reply->str);
    freeReplyObject(reply);
}

void changeDatabase(KEY_TYPE type) {
    if(type == BLOCK) {
        redisCommand(client.context,"SELECT 0");
    } else if(type == PEER) {
        redisCommand(client.context,"SELECT 1");
    }
}

void clean_registry(KEY_TYPE type) {
    changeDatabase(type);
    redisCommand(client.context,"FLUSHDB");
}

// Format