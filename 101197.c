  ~ScopedMMap() {
    if (munmap(mem_, len_) == -1) {
      LOG_ERRNO("%s: munmap failed when trying to unmap zip file\n",
                __FUNCTION__);
    }
  }
