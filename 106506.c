  char *strdup(const char *s)
  {
    size_t lb = strlen(s) + 1;
    char *result = (char *)REDIRECT_MALLOC(lb);
    if (result == 0) {
      errno = ENOMEM;
      return 0;
    }
    BCOPY(s, result, lb);
    return result;
  }
