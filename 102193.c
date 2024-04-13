const void* Parcel::readInplace(size_t len) const
{
 if (len > INT32_MAX) {
 return NULL;
 }

 if ((mDataPos+pad_size(len)) >= mDataPos && (mDataPos+pad_size(len)) <= mDataSize
 && len <= pad_size(len)) {
 const void* data = mData+mDataPos;
        mDataPos += pad_size(len);
        ALOGV("readInplace Setting data pos of %p to %zu", this, mDataPos);
 return data;
 }
 return NULL;
}
