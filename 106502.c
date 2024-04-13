  STATIC void GC_init_lib_bounds(void)
  {
    if (GC_libpthread_start != 0) return;
    GC_init(); /* if not called yet */
    if (!GC_text_mapping("libpthread-",
                         &GC_libpthread_start, &GC_libpthread_end)) {
        WARN("Failed to find libpthread.so text mapping: Expect crash\n", 0);
        /* This might still work with some versions of libpthread,      */
        /* so we don't abort.  Perhaps we should.                       */
        /* Generate message only once:                                  */
          GC_libpthread_start = (ptr_t)1;
    }
    if (!GC_text_mapping("ld-", &GC_libld_start, &GC_libld_end)) {
        WARN("Failed to find ld.so text mapping: Expect crash\n", 0);
    }
   }
