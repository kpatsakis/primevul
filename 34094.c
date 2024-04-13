GetTimeInMillis(void)
{
    struct timeval tv;

#ifdef MONOTONIC_CLOCK
    struct timespec tp;
    static clockid_t clockid;
    if (!clockid) {
#ifdef CLOCK_MONOTONIC_COARSE
        if (clock_getres(CLOCK_MONOTONIC_COARSE, &tp) == 0 &&
            (tp.tv_nsec / 1000) <= 1000 &&
            clock_gettime(CLOCK_MONOTONIC_COARSE, &tp) == 0)
            clockid = CLOCK_MONOTONIC_COARSE;
        else
#endif
        if (clock_gettime(CLOCK_MONOTONIC, &tp) == 0)
            clockid = CLOCK_MONOTONIC;
        else
            clockid = ~0L;
    }
    if (clockid != ~0L && clock_gettime(clockid, &tp) == 0)
        return (tp.tv_sec * 1000) + (tp.tv_nsec / 1000000L);
#endif

    X_GETTIMEOFDAY(&tv);
    return(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
