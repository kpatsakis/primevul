void QuicStreamSequencerBuffer::ReleaseWholeBuffer() {
  Clear();
  blocks_.reset(nullptr);
}
