void* IMemory::fastPointer(const sp<IBinder>& binder, ssize_t offset) const
{
    sp<IMemoryHeap> realHeap = BpMemoryHeap::get_heap(binder);
 void* const base = realHeap->base();
 if (base == MAP_FAILED)
 return 0;
 return static_cast<char*>(base) + offset;
}
