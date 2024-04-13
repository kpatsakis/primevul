const char* Parcel::readCString() const
{
 const size_t avail = mDataSize-mDataPos;
 if (avail > 0) {
 const char* str = reinterpret_cast<const char*>(mData+mDataPos);
 const char* eos = reinterpret_cast<const char*>(memchr(str, 0, avail));
 if (eos) {
 const size_t len = eos - str;
            mDataPos += pad_size(len+1);
            ALOGV("readCString Setting data pos of %p to %zu", this, mDataPos);
 return str;
 }
 }
 return NULL;
}
