 uint32_t next() {
 if (!mCount)
 return 0;
        mCount--;
 return (mNext++ % mSize) + mMin;
 }
