char *av_strdup(const char *s)
{
    char *ptr = NULL;
    if (s) {
        int len = strlen(s) + 1;
        ptr = av_malloc(len);
        if (ptr)
            memcpy(ptr, s, len);
    }
    return ptr;
}
