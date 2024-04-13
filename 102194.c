void release_object(const sp<ProcessState>& proc,
 const flat_binder_object& obj, const void* who)
{
    release_object(proc, obj, who, NULL);
}
