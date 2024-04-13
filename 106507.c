  char *strndup(const char *str, size_t size)
  {
    char *copy;
    size_t len = strlen(str);
    if (len > size)
      len = size;
    copy = (char *)REDIRECT_MALLOC(len + 1);
    if (copy == NULL) {
      errno = ENOMEM;
      return NULL;
    }
    BCOPY(str, copy, len);
    copy[len] = '\0';
    return copy;
  }
