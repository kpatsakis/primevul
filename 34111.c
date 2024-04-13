XNFstrdup(const char *s)
{
    char *ret;

    if (s == NULL)
	return NULL;

    ret = strdup(s);
    if (!ret)
	FatalError("XNFstrdup: Out of memory");
    return ret;
}
