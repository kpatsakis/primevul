  GC_API wchar_t * GC_CALL GC_wcsdup(const wchar_t *str)
  {
    size_t lb = (wcslen(str) + 1) * sizeof(wchar_t);
    wchar_t *copy = GC_malloc_atomic(lb);
    if (copy == NULL) {
#     ifndef MSWINCE
        errno = ENOMEM;
#     endif
      return NULL;
    }
    BCOPY(str, copy, lb);
    return copy;
  }
