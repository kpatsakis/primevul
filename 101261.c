LocalFrame* Document::GetFrameOfMasterDocument() const {
  if (frame_)
    return frame_;
  if (imports_controller_)
    return imports_controller_->Master()->GetFrame();
  return nullptr;
}
