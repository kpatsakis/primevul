void MPEG4DataSource::clearCache() {
 if (mCache) {
        free(mCache);
        mCache = NULL;
 }

    mCachedOffset = 0;
    mCachedSize = 0;
}
