AudioBuffer* BaseAudioContext::createBuffer(uint32_t number_of_channels,
                                            uint32_t number_of_frames,
                                            float sample_rate,
                                            ExceptionState& exception_state) {

  AudioBuffer* buffer = AudioBuffer::Create(
      number_of_channels, number_of_frames, sample_rate, exception_state);

  if (buffer) {
    DEFINE_STATIC_LOCAL(SparseHistogram, audio_buffer_channels_histogram,
                        ("WebAudio.AudioBuffer.NumberOfChannels"));

    DEFINE_STATIC_LOCAL(CustomCountHistogram, audio_buffer_length_histogram,
                        ("WebAudio.AudioBuffer.Length", 1, 1000000, 50));
    DEFINE_STATIC_LOCAL(
        CustomCountHistogram, audio_buffer_sample_rate_histogram,
        ("WebAudio.AudioBuffer.SampleRate384kHz", 3000, 384000, 60));

    audio_buffer_channels_histogram.Sample(number_of_channels);
    audio_buffer_length_histogram.Count(number_of_frames);
    audio_buffer_sample_rate_histogram.Count(sample_rate);

    if (!IsContextClosed()) {
      DEFINE_STATIC_LOCAL(
          CustomCountHistogram, audio_buffer_sample_rate_ratio_histogram,
          ("WebAudio.AudioBuffer.SampleRateRatio384kHz", 1, 12800, 50));
      float ratio = 100 * sample_rate / this->sampleRate();
      audio_buffer_sample_rate_ratio_histogram.Count(
          static_cast<int>(0.5 + ratio));
    }
  }

  return buffer;
}
