Xrealloc(void *ptr, unsigned long amount)
{
    /*
     * Xrealloc used to return NULL when large amount of memory is requested. In
     * order to catch the buggy callers this warning has been added, slated to
     * removal by anyone who touches this code (or just looks at it) in 2011.
     *
     * -- Mikhail Gusarov
     */
    if ((long)amount <= 0)
	ErrorF("Warning: Xrealloc: "
	       "requesting unpleasantly large amount of memory: %lu bytes.\n",
               amount);

    return realloc(ptr, amount);
}
