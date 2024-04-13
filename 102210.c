void AudioSource::queueInputBuffer_l(MediaBuffer *buffer, int64_t timeUs) {
 const size_t bufferSize = buffer->range_length();
 const size_t frameSize = mRecord->frameSize();
 const int64_t timestampUs =
                mPrevSampleTimeUs +
 ((1000000LL * (bufferSize / frameSize)) +
 (mSampleRate >> 1)) / mSampleRate;

 if (mNumFramesReceived == 0) {
        buffer->meta_data()->setInt64(kKeyAnchorTime, mStartTimeUs);
 }

    buffer->meta_data()->setInt64(kKeyTime, mPrevSampleTimeUs);
    buffer->meta_data()->setInt64(kKeyDriftTime, timeUs - mInitialReadTimeUs);
    mPrevSampleTimeUs = timestampUs;
    mNumFramesReceived += bufferSize / frameSize;
    mBuffersReceived.push_back(buffer);
    mFrameAvailableCondition.signal();
}
