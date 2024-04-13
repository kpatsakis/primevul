bool HTMLMediaElement::PotentiallyPlaying() const {
  bool paused_to_buffer =
      ready_state_maximum_ >= kHaveFutureData && ready_state_ < kHaveFutureData;
  return (paused_to_buffer || ready_state_ >= kHaveFutureData) &&
         CouldPlayIfEnoughData();
}
