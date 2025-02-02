status_t MyVorbisExtractor::verifyHeader(
 MediaBuffer *buffer, uint8_t type) {
 const uint8_t *data =
 (const uint8_t *)buffer->data() + buffer->range_offset();

 size_t size = buffer->range_length();

 if (size < 7 || data[0] != type || memcmp(&data[1], "vorbis", 6)) {
 return ERROR_MALFORMED;
 }

    ogg_buffer buf;
    buf.data = (uint8_t *)data;
    buf.size = size;
    buf.refcount = 1;
    buf.ptr.owner = NULL;

    ogg_reference ref;
    ref.buffer = &buf;
    ref.begin = 0;
    ref.length = size;
    ref.next = NULL;

    oggpack_buffer bits;
    oggpack_readinit(&bits, &ref);

 if (oggpack_read(&bits, 8) != type) {
 return ERROR_MALFORMED;
 }
 for (size_t i = 0; i < 6; ++i) {
        oggpack_read(&bits, 8); // skip 'vorbis'
 }

 switch (type) {
 case 1:
 {
 if (0 != _vorbis_unpack_info(&mVi, &bits)) {
 return ERROR_MALFORMED;
 }

            mMeta->setData(kKeyVorbisInfo, 0, data, size);
            mMeta->setInt32(kKeySampleRate, mVi.rate);
            mMeta->setInt32(kKeyChannelCount, mVi.channels);

            ALOGV("lower-bitrate = %ld", mVi.bitrate_lower);
            ALOGV("upper-bitrate = %ld", mVi.bitrate_upper);
            ALOGV("nominal-bitrate = %ld", mVi.bitrate_nominal);
            ALOGV("window-bitrate = %ld", mVi.bitrate_window);
            ALOGV("blocksizes: %d/%d",
                    vorbis_info_blocksize(&mVi, 0),
                    vorbis_info_blocksize(&mVi, 1)
 );

 off64_t size;
 if (mSource->getSize(&size) == OK) {
 uint64_t bps = approxBitrate();
 if (bps != 0) {
                    mMeta->setInt64(kKeyDuration, size * 8000000ll / bps);
 }
 }
 break;
 }

 case 3:
 {
 if (0 != _vorbis_unpack_comment(&mVc, &bits)) {
 return ERROR_MALFORMED;
 }

            parseFileMetaData();
 break;
 }

 case 5:
 {
 if (0 != _vorbis_unpack_books(&mVi, &bits)) {
 return ERROR_MALFORMED;
 }

            mMeta->setData(kKeyVorbisBooks, 0, data, size);
 break;
 }
 }

 return OK;
}
