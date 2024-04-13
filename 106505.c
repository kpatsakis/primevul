void * malloc(size_t lb)
{
    /* It might help to manually inline the GC_malloc call here.        */
    /* But any decent compiler should reduce the extra procedure call   */
    /* to at most a jump instruction in this case.                      */
#   if defined(I386) && defined(GC_SOLARIS_THREADS)
      /*
       * Thread initialisation can call malloc before
       * we're ready for it.
       * It's not clear that this is enough to help matters.
       * The thread implementation may well call malloc at other
       * inopportune times.
       */
      if (!EXPECT(GC_is_initialized, TRUE)) return sbrk(lb);
#   endif /* I386 && GC_SOLARIS_THREADS */
    return((void *)REDIRECT_MALLOC(lb));
}
