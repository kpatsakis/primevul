  void free(void * p)
  {
#   if defined(GC_LINUX_THREADS) && !defined(USE_PROC_FOR_LIBRARIES)
        {
          /* Don't bother with initialization checks.  If nothing       */
          /* has been initialized, the check fails, and that's safe,    */
          /* since we haven't allocated uncollectable objects either.   */
          ptr_t caller = (ptr_t)__builtin_return_address(0);
          /* This test does not need to ensure memory visibility, since */
          /* the bounds will be set when/if we create another thread.   */
          if (((word)caller >= (word)GC_libpthread_start
               && (word)caller < (word)GC_libpthread_end)
              || ((word)caller >= (word)GC_libld_start
                  && (word)caller < (word)GC_libld_end)) {
            GC_free(p);
            return;
          }
        }
#   endif
#   ifndef IGNORE_FREE
      REDIRECT_FREE(p);
#   endif
  }
