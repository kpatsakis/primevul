static int cliSelect(void) {
    redisReply *reply;
    if (config.dbnum == 0) return REDIS_OK;

    reply = redisCommand(context,"SELECT %d",config.dbnum);
    if (reply != NULL) {
        int result = REDIS_OK;
        if (reply->type == REDIS_REPLY_ERROR) result = REDIS_ERR;
        freeReplyObject(reply);
        return result;
    }
    return REDIS_ERR;
}
