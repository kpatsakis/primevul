xstrtokenize(const char *str, const char *separators)
{
    char **list, **nlist;
    char *tok, *tmp;
    unsigned num = 0, n;

    if (!str)
        return NULL;
    list = calloc(1, sizeof(*list));
    if (!list)
        return NULL;
    tmp = strdup(str);
    if (!tmp)
        goto error;
    for (tok = strtok(tmp, separators); tok; tok = strtok(NULL, separators)) {
        nlist = realloc(list, (num + 2) * sizeof(*list));
        if (!nlist)
            goto error;
        list = nlist;
        list[num] = strdup(tok);
        if (!list[num])
            goto error;
        list[++num] = NULL;
    }
    free(tmp);
    return list;

error:
    free(tmp);
    for (n = 0; n < num; n++)
        free(list[n]);
    free(list);
    return NULL;
}
