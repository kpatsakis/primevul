status_t Parcel::writeObject(const flat_binder_object& val, bool nullMetaData)
{
 const bool enoughData = (mDataPos+sizeof(val)) <= mDataCapacity;
 const bool enoughObjects = mObjectsSize < mObjectsCapacity;
 if (enoughData && enoughObjects) {
restart_write:
 *reinterpret_cast<flat_binder_object*>(mData+mDataPos) = val;

 if (nullMetaData || val.binder != 0) {
            mObjects[mObjectsSize] = mDataPos;
            acquire_object(ProcessState::self(), val, this);
            mObjectsSize++;
 }

 if (val.type == BINDER_TYPE_FD) {
 if (!mAllowFds) {
 return FDS_NOT_ALLOWED;
 }
            mHasFds = mFdsKnown = true;
 }

 return finishWrite(sizeof(flat_binder_object));
 }

 if (!enoughData) {
 const status_t err = growData(sizeof(val));
 if (err != NO_ERROR) return err;
 }
 if (!enoughObjects) {
 size_t newSize = ((mObjectsSize+2)*3)/2;
 binder_size_t* objects = (binder_size_t*)realloc(mObjects, newSize*sizeof(binder_size_t));
 if (objects == NULL) return NO_MEMORY;
        mObjects = objects;
        mObjectsCapacity = newSize;
 }

 goto restart_write;
}
