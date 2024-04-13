ACodec::BufferInfo *ACodec::findBufferByID(
 uint32_t portIndex, IOMX::buffer_id bufferID, ssize_t *index) {
 for (size_t i = 0; i < mBuffers[portIndex].size(); ++i) {
 BufferInfo *info = &mBuffers[portIndex].editItemAt(i);

 if (info->mBufferID == bufferID) {
 if (index != NULL) {
 *index = i;
 }
 return info;
 }
 }

    ALOGE("Could not find buffer with ID %u", bufferID);
 return NULL;
}
