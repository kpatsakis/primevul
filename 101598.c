bool RendererSchedulerImpl::ShouldDisableThrottlingBecauseOfAudio(
    base::TimeTicks now) {
  if (!main_thread_only().last_audio_state_change)
    return false;

  if (main_thread_only().is_audio_playing)
    return true;

  return main_thread_only().last_audio_state_change.value() +
             kThrottlingDelayAfterAudioIsPlayed >
         now;
}
