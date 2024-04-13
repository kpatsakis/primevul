OsAbort (void)
{
#ifndef __APPLE__
    OsBlockSignals();
#endif
    abort();
}
