GC_API char * GC_CALL GC_strndup(const char *str, size_t size)
{
  char *copy;
  size_t len = strlen(str); /* str is expected to be non-NULL  */
  if (len > size)
    len = size;
  copy = GC_malloc_atomic(len + 1);
  if (copy == NULL) {
#   ifndef MSWINCE
      errno = ENOMEM;
#   endif
    return NULL;
  }
  BCOPY(str, copy, len);
  copy[len] = '\0';
  return copy;
}
