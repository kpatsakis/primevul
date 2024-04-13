 explicit MyOpusExtractor(const sp<DataSource> &source)
 : MyOggExtractor(source, MEDIA_MIMETYPE_AUDIO_OPUS, /*numHeaders*/ 2, kOpusSeekPreRollUs),
          mChannelCount(0),
          mCodecDelay(0),
          mStartGranulePosition(-1) {
 }
