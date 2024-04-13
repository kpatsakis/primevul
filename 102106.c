void Parcel::releaseObjects()
{
 const sp<ProcessState> proc(ProcessState::self());
 size_t i = mObjectsSize;
 uint8_t* const data = mData;
 binder_size_t* const objects = mObjects;
 while (i > 0) {
        i--;
 const flat_binder_object* flat
 = reinterpret_cast<flat_binder_object*>(data+objects[i]);
        release_object(proc, *flat, this);
 }
}
