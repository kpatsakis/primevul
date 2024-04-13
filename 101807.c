void AudioHandler::ProcessIfNecessary(size_t frames_to_process) {
  DCHECK(Context()->IsAudioThread());

  if (!IsInitialized())
    return;

  double current_time = Context()->currentTime();
  if (last_processing_time_ != current_time) {
    last_processing_time_ = current_time;

    PullInputs(frames_to_process);

    bool silent_inputs = InputsAreSilent();
    if (!silent_inputs) {
      last_non_silent_time_ =
          (Context()->CurrentSampleFrame() + frames_to_process) /
          static_cast<double>(Context()->sampleRate());
    }

    if (silent_inputs && PropagatesSilence()) {
      SilenceOutputs();
      ProcessOnlyAudioParams(frames_to_process);
    } else {
      UnsilenceOutputs();
      Process(frames_to_process);
    }
  }
}
