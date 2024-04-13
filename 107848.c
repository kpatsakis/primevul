void * realloc(void * p, size_t lb)
  {
    return(REDIRECT_REALLOC(p, lb));
  }
