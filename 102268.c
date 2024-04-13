void HeapCache::binderDied(const wp<IBinder>& binder)
{
    free_heap(binder);
}
