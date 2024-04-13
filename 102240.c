void IPCThreadState::expungeHandle(int32_t handle, IBinder* binder)
{
#if LOG_REFCOUNTS
    printf("IPCThreadState::expungeHandle(%ld)\n", handle);
#endif
    self()->mProcess->expungeHandle(handle, binder);
}
