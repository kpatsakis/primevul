status_t MyOpusExtractor::verifyOpusComments(MediaBuffer *buffer) {
 int32_t commentSize = buffer->range_length() + 1;
    sp<ABuffer> aBuf = new ABuffer(commentSize);
 if (aBuf->capacity() <= buffer->range_length()) {
 return ERROR_MALFORMED;
 }

 uint8_t* commentData = aBuf->data();
    memcpy(commentData,
 (uint8_t *)buffer->data() + buffer->range_offset(),
            buffer->range_length());

    ogg_buffer buf;
    buf.data = commentData;
    buf.size = commentSize;
    buf.refcount = 1;
    buf.ptr.owner = NULL;

    ogg_reference ref;
    ref.buffer = &buf;
    ref.begin = 0;
    ref.length = commentSize;
    ref.next = NULL;

    oggpack_buffer bits;
    oggpack_readinit(&bits, &ref);

 const char *OpusTags = "OpusTags";
 const int32_t headerLen = strlen(OpusTags);
 int32_t framingBitOffset = headerLen;
 for (int i = 0; i < headerLen; ++i) {
 char chr = oggpack_read(&bits, 8);
 if (chr != OpusTags[i]) {
 return ERROR_MALFORMED;
 }
 }

 int32_t vendorLen = oggpack_read(&bits, 32);
    framingBitOffset += 4;
 if (vendorLen < 0 || vendorLen > commentSize - 8) {
 return ERROR_MALFORMED;
 }
    framingBitOffset += vendorLen;
 for (int i = 0; i < vendorLen; ++i) {
        oggpack_read(&bits, 8);
 }

 int32_t n = oggpack_read(&bits, 32);
    framingBitOffset += 4;
 if (n < 0 || n > ((commentSize - oggpack_bytes(&bits)) >> 2)) {
 return ERROR_MALFORMED;
 }
 for (int i = 0; i < n; ++i) {
 int32_t len = oggpack_read(&bits, 32);
        framingBitOffset += 4;
 if (len  < 0 || len  > (commentSize - oggpack_bytes(&bits))) {
 return ERROR_MALFORMED;
 }
        framingBitOffset += len;
 for (int j = 0; j < len; ++j) {
            oggpack_read(&bits, 8);
 }
 }
 if (framingBitOffset < 0 || framingBitOffset >= commentSize) {
 return ERROR_MALFORMED;
 }
    commentData[framingBitOffset] = 1;

    buf.data = commentData + headerLen;
    buf.size = commentSize - headerLen;
    buf.refcount = 1;
    buf.ptr.owner = NULL;

    ref.buffer = &buf;
    ref.begin = 0;
    ref.length = commentSize - headerLen;
    ref.next = NULL;

    oggpack_readinit(&bits, &ref);
 int err = _vorbis_unpack_comment(&mVc, &bits);
 if (0 != err) {
 return ERROR_MALFORMED;
 }

    parseFileMetaData();
 return OK;
}
