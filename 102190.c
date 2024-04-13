status_t Parcel::continueWrite(size_t desired)
{
 if (desired > INT32_MAX) {
 return BAD_VALUE;
 }

 size_t objectsSize = mObjectsSize;
 if (desired < mDataSize) {
 if (desired == 0) {
            objectsSize = 0;
 } else {
 while (objectsSize > 0) {
 if (mObjects[objectsSize-1] < desired)
 break;
                objectsSize--;
 }
 }
 }

 if (mOwner) {
 if (desired == 0) {
            freeData();
 return NO_ERROR;
 }

 uint8_t* data = (uint8_t*)malloc(desired);
 if (!data) {
            mError = NO_MEMORY;
 return NO_MEMORY;
 }
 binder_size_t* objects = NULL;

 if (objectsSize) {
            objects = (binder_size_t*)calloc(objectsSize, sizeof(binder_size_t));
 if (!objects) {
                free(data);

                mError = NO_MEMORY;
 return NO_MEMORY;
 }

 size_t oldObjectsSize = mObjectsSize;
            mObjectsSize = objectsSize;
            acquireObjects();
            mObjectsSize = oldObjectsSize;
 }

 if (mData) {
            memcpy(data, mData, mDataSize < desired ? mDataSize : desired);
 }
 if (objects && mObjects) {
            memcpy(objects, mObjects, objectsSize*sizeof(binder_size_t));
 }
        mOwner(this, mData, mDataSize, mObjects, mObjectsSize, mOwnerCookie);
        mOwner = NULL;

        LOG_ALLOC("Parcel %p: taking ownership of %zu capacity", this, desired);
        pthread_mutex_lock(&gParcelGlobalAllocSizeLock);
        gParcelGlobalAllocSize += desired;
        gParcelGlobalAllocCount++;
        pthread_mutex_unlock(&gParcelGlobalAllocSizeLock);

        mData = data;
        mObjects = objects;
        mDataSize = (mDataSize < desired) ? mDataSize : desired;
        ALOGV("continueWrite Setting data size of %p to %zu", this, mDataSize);
        mDataCapacity = desired;
        mObjectsSize = mObjectsCapacity = objectsSize;
        mNextObjectHint = 0;

 } else if (mData) {
 if (objectsSize < mObjectsSize) {
 const sp<ProcessState> proc(ProcessState::self());
 for (size_t i=objectsSize; i<mObjectsSize; i++) {
 const flat_binder_object* flat
 = reinterpret_cast<flat_binder_object*>(mData+mObjects[i]);
 if (flat->type == BINDER_TYPE_FD) {
                    mFdsKnown = false;
 }
                release_object(proc, *flat, this, &mOpenAshmemSize);
 }
 binder_size_t* objects =
 (binder_size_t*)realloc(mObjects, objectsSize*sizeof(binder_size_t));
 if (objects) {
                mObjects = objects;
 }
            mObjectsSize = objectsSize;
            mNextObjectHint = 0;
 }

 if (desired > mDataCapacity) {
 uint8_t* data = (uint8_t*)realloc(mData, desired);
 if (data) {
                LOG_ALLOC("Parcel %p: continue from %zu to %zu capacity", this, mDataCapacity,
                        desired);
                pthread_mutex_lock(&gParcelGlobalAllocSizeLock);
                gParcelGlobalAllocSize += desired;
                gParcelGlobalAllocSize -= mDataCapacity;
                pthread_mutex_unlock(&gParcelGlobalAllocSizeLock);
                mData = data;
                mDataCapacity = desired;
 } else if (desired > mDataCapacity) {
                mError = NO_MEMORY;
 return NO_MEMORY;
 }
 } else {
 if (mDataSize > desired) {
                mDataSize = desired;
                ALOGV("continueWrite Setting data size of %p to %zu", this, mDataSize);
 }
 if (mDataPos > desired) {
                mDataPos = desired;
                ALOGV("continueWrite Setting data pos of %p to %zu", this, mDataPos);
 }
 }

 } else {
 uint8_t* data = (uint8_t*)malloc(desired);
 if (!data) {
            mError = NO_MEMORY;
 return NO_MEMORY;
 }

 if(!(mDataCapacity == 0 && mObjects == NULL
 && mObjectsCapacity == 0)) {
            ALOGE("continueWrite: %zu/%p/%zu/%zu", mDataCapacity, mObjects, mObjectsCapacity, desired);
 }

        LOG_ALLOC("Parcel %p: allocating with %zu capacity", this, desired);
        pthread_mutex_lock(&gParcelGlobalAllocSizeLock);
        gParcelGlobalAllocSize += desired;
        gParcelGlobalAllocCount++;
        pthread_mutex_unlock(&gParcelGlobalAllocSizeLock);

        mData = data;
        mDataSize = mDataPos = 0;
        ALOGV("continueWrite Setting data size of %p to %zu", this, mDataSize);
        ALOGV("continueWrite Setting data pos of %p to %zu", this, mDataPos);
        mDataCapacity = desired;
 }

 return NO_ERROR;
}
