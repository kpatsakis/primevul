status_t Camera3Device::tryRemoveDummyStreamLocked() {
    ATRACE_CALL();
 status_t res;

 if (mDummyStreamId == NO_STREAM) return OK;
 if (mOutputStreams.size() == 1) return OK;

    ALOGV("%s: Camera %d: Removing the dummy stream", __FUNCTION__, mId);


    sp<Camera3StreamInterface> deletedStream;
 ssize_t outputStreamIdx = mOutputStreams.indexOfKey(mDummyStreamId);
 if (outputStreamIdx == NAME_NOT_FOUND) {
        SET_ERR_L("Dummy stream %d does not appear to exist", mDummyStreamId);
 return INVALID_OPERATION;
 }

    deletedStream = mOutputStreams.editValueAt(outputStreamIdx);
    mOutputStreams.removeItemsAt(outputStreamIdx);

    res = deletedStream->disconnect();
 if (res != OK) {
        SET_ERR_L("Can't disconnect deleted dummy stream %d", mDummyStreamId);
 }
    mDeletedStreams.add(deletedStream);
    mDummyStreamId = NO_STREAM;

 return res;
}
