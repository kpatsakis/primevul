void updateCachedTime(void) {
    server.unixtime = time(NULL);
    server.mstime = mstime();
}
