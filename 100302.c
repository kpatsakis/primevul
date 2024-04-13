void stopSharedTimer()
{
    gboolean s = FALSE;
    if (sharedTimer == 0)
        return;

    s = g_source_remove(sharedTimer);
    ASSERT(s);
    sharedTimer = 0;
}
