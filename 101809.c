void AudioHandler::SetChannelInterpretation(const String& interpretation,
                                            ExceptionState& exception_state) {
  DCHECK(IsMainThread());
  BaseAudioContext::GraphAutoLocker locker(Context());

  AudioBus::ChannelInterpretation old_mode = channel_interpretation_;

  if (interpretation == "speakers") {
    new_channel_interpretation_ = AudioBus::kSpeakers;
  } else if (interpretation == "discrete") {
    new_channel_interpretation_ = AudioBus::kDiscrete;
  } else {
    NOTREACHED();
  }

  if (new_channel_interpretation_ != old_mode)
    Context()->GetDeferredTaskHandler().AddChangedChannelInterpretation(this);
}
