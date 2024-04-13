static int toIntType(char *key, char *type) {
    if(!strcmp(type, "string")) {
        return TYPE_STRING;
    } else if(!strcmp(type, "list")) {
        return TYPE_LIST;
    } else if(!strcmp(type, "set")) {
        return TYPE_SET;
    } else if(!strcmp(type, "hash")) {
        return TYPE_HASH;
    } else if(!strcmp(type, "zset")) {
        return TYPE_ZSET;
    } else if(!strcmp(type, "none")) {
        return TYPE_NONE;
    } else {
        fprintf(stderr, "Unknown type '%s' for key '%s'\n", type, key);
        exit(1);
    }
}
