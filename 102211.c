void AudioSource::rampVolume(
 int32_t startFrame, int32_t rampDurationFrames,
 uint8_t *data, size_t bytes) {

 const int32_t kShift = 14;
 int32_t fixedMultiplier = (startFrame << kShift) / rampDurationFrames;
 const int32_t nChannels = mRecord->channelCount();
 int32_t stopFrame = startFrame + bytes / sizeof(int16_t);
 int16_t *frame = (int16_t *) data;
 if (stopFrame > rampDurationFrames) {
        stopFrame = rampDurationFrames;
 }

 while (startFrame < stopFrame) {
 if (nChannels == 1) { // mono
            frame[0] = (frame[0] * fixedMultiplier) >> kShift;
 ++frame;
 ++startFrame;
 } else { // stereo
            frame[0] = (frame[0] * fixedMultiplier) >> kShift;
            frame[1] = (frame[1] * fixedMultiplier) >> kShift;
            frame += 2;
            startFrame += 2;
 }

 if ((startFrame & 3) == 0) {
            fixedMultiplier = (startFrame << kShift) / rampDurationFrames;
 }
 }
}
