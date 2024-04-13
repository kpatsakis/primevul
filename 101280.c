LocalFrameView* Document::View() const {
  return frame_ ? frame_->View() : nullptr;
}
