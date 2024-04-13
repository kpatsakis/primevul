struct redisCommand *lookupCommand(sds name) {
    return dictFetchValue(server.commands, name);
}
